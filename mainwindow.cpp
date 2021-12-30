#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

GRANSAC::VPFloat Slope(int x0, int y0, int x1, int y1)
{
    return (GRANSAC::VPFloat)(y1 - y0) / (x1 - x0);
}

void DrawFullLine(cv::Mat& img, cv::Point a, cv::Point b, cv::Scalar color, int LineWidth)
{
    GRANSAC::VPFloat slope = Slope(a.x, a.y, b.x, b.y);

    cv::Point p(0, 0), q(img.cols, img.rows);

    p.y = -(a.x - p.x) * slope + a.y;
    q.y = -(b.x - q.x) * slope + b.y;

    cv::line(img, p, q, color, LineWidth, cv::LINE_AA, 0);
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->display1->setScaledContents(true);

    ui->display1->setFixedSize( Side, Side );
    ui->display2->setScaledContents(true);

    ui->display2->setFixedSize( Side, Side );
    statusBar()->showMessage("Press Execute to start the simulation");
    ui->threadComboBox->clear();
    for (int i = 1; i <= omp_get_max_threads(); i++){
        ui->threadComboBox->addItem(QString::number(i));
    }
    std::random_device SeedDevice;
    RNG = std::mt19937(SeedDevice());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update(float execTime){

    QImage img1 = MatToQImage( Canvas );
    QImage img2 = MatToQImage( Canvas_init );
    int nThreads = (ui->threadComboBox->currentText()).toInt();
    ui->display1->setPixmap(QPixmap::fromImage(img1));
    ui->display2->setPixmap(QPixmap::fromImage(img2));
    statusBar()->showMessage("GRANSAC finished using " + QString::number(nThreads) + " threads in " +
                             QString::number(execTime) + "ms");

}

void MainWindow::execute(){
    int maxThread = (ui->threadComboBox->currentText()).toInt();
    int treshold = ui->tresholdSpinBox->value();
    int iter = ui->iterationSpinBox->value();
    //Mat Canvas(Side, Side, CV_8UC3);
    Mat Canvas_temp(Size(Side, Side), CV_8UC3);

    Canvas_temp.setTo(255);
    Canvas_temp.copyTo(Canvas);
    Canvas_temp.copyTo(Canvas_init);

    setWindowTitle(QString("GRANSAC visualize using OpenCV V %1.%2").arg(CV_MAJOR_VERSION).arg(CV_MINOR_VERSION));
    // Randomly generate points in a 2D plane roughly aligned in a line for testing
//    std::random_device SeedDevice;
//    std::mt19937 RNG = std::mt19937(SeedDevice());

    std::uniform_int_distribution<int> UniDist(0, Side - 1); // [Incl, Incl]
    int Perturb = ui->perturbSpinBox->value();
    std::normal_distribution<GRANSAC::VPFloat> PerturbDist(0, Perturb);

    std::vector<std::shared_ptr<GRANSAC::AbstractParameter>> CandPoints;
    for (int i = 0; i < nPoints; ++i)
    {
        int Diag = UniDist(RNG);
        cv::Point Pt(floor(Diag + PerturbDist(RNG)), floor(Diag + PerturbDist(RNG)));
        cv::circle(Canvas, Pt, floor(Side / 100) + 3, cv::Scalar(0, 0, 0), 2, cv::LINE_AA);
        cv::circle(Canvas_init, Pt, floor(Side / 100) + 3, cv::Scalar(0, 0, 0), 2, cv::LINE_AA);

        std::shared_ptr<GRANSAC::AbstractParameter> CandPt = std::make_shared<Point2D>(Pt.x, Pt.y);
        CandPoints.push_back(CandPt);
    }

    GRANSAC::RANSAC<Line2DModel, 2> Estimator;
    Estimator.Initialize(treshold, iter); // Threshold, iterations
    Estimator.setMaxThreads(maxThread);
    int64_t start = cv::getTickCount();
    Estimator.Estimate(CandPoints);
    int64_t end = cv::getTickCount();
    float execTime = GRANSAC::VPFloat(end - start) / GRANSAC::VPFloat(cv::getTickFrequency()) * 1000.0;


    auto BestInliers = Estimator.GetBestInliers();
    if (BestInliers.size() > 0)
    {
        for (auto& Inlier : BestInliers)
        {
            auto RPt = std::dynamic_pointer_cast<Point2D>(Inlier);
            cv::Point Pt(floor(RPt->m_Point2D[0]), floor(RPt->m_Point2D[1]));
            cv::circle(Canvas, Pt, floor(Side / 100), cv::Scalar(0, 255, 0), -1, cv::LINE_AA);
        }
    }

    auto BestLine = Estimator.GetBestModel();
    if (BestLine)
    {
        auto BestLinePt1 = std::dynamic_pointer_cast<Point2D>(BestLine->GetModelParams()[0]);
        auto BestLinePt2 = std::dynamic_pointer_cast<Point2D>(BestLine->GetModelParams()[1]);
        if (BestLinePt1 && BestLinePt2)
        {
            cv::Point Pt1(BestLinePt1->m_Point2D[0], BestLinePt1->m_Point2D[1]);
            cv::Point Pt2(BestLinePt2->m_Point2D[0], BestLinePt2->m_Point2D[1]);
            DrawFullLine(Canvas, Pt1, Pt2, cv::Scalar(0, 0, 255), 2);
        }
    }

    //cv::imwrite("LineFitting.png", Canvas);
    // Call update every 100ms
    update(execTime);

}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Random Sampling Consensus (RANSAC) using multi core\nOpen Parameter\n\nDescription of the program\n");
    msgBox.setInformativeText("RANSAC algorithm is an itterative algorithm that tries to predict a line that can represent the most points in a data set."
                              "It will randomly take subsets and then predict the line equation and then test it against the reminders data set."
                              "If the result still below or in the treshold, a point counted as inliers."
                              "After several itterations, it will return the line that represent the most points / the most inliers"
                              "\n\nLeft picture indicate raw data, right picture indicate the inliers (green circle) and predicted line"
                              "\n\nSettings Parameter Definition:"
                              "\n1. Number of Thread: Number of thread used to compute the iteration"
                              "\n2. Threshold: Threshold for determining the inliers"
                              "\n3. Max Iter: Number of iteration for the RANSAC"
                              "\n4. Pertubation: Describing how distributed the data"
                              "\n\nMichael Jonathan, Swiss German University 2021");
    msgBox.exec();
}


void MainWindow::on_pushButtonExecute_clicked()
{
    execute();
}


