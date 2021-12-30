# QT5-GRANSAC
Multi threaded generic RANSAC implementation and visualized using QT5.



## RANSAC

### Definition
RANSAC (Random Sample Consensus) algorithm used in a lot of robotic application. It is mainly used to pre-process the incoming LIDAR data to remove the possible outliers. Most common application where RANSAC algorithm is being used is detecting a surface.

### How It Works
RANSAC algorithm is an itterative algorithm that tries to predict a line that can represent the most points in a data set.
It will randomly take subsets and then predict the line equation and then test it against the reminders data set.
If the result still below or in the treshold, a point counted as inliers.
After several itterations, it will return the line that represent the most points / the most inliers



## QT5-GUI
![Alt text](https://github.com/mich1342/QT5-GRANSAC/blob/main/ScreeenShoots.png?raw=true "Screenshoots of the gui")

Settings the parameter first and then press the execute button to simulate the RANSAC algorithm.

At the left part of the image, the raw data set is shown. At the right part of the image, the iliers are marked with green color and the estimated line marked with red line.



## Settings Parameter

### 1. Number of Thread
Number of thread used to compute and iterrate.

### 2. Threshold
Threshold for determining the inliers in the RANSAC algorithm.

### 3. Max Iter
Maximum number of iterration for the RANSAC algorithm.

### 4. Pertubation
Describing how distributed the data.



## Credits and Contribution
### 1. The algorithm was addopted from drsrinathsridhar's [GRANSAC](https://github.com/drsrinathsridhar/GRANSAC) repo.
- Convert cmake into qmake.

- Add QT5 gui to visualize the result clearly.

- Modified the code so that some parameters can be add add changed by using QT5 widgets.

### 2. The basic GUI was adopted rom rusmanr's [DSP_OpenCV](https://github.com/rusmanr/DSP-OpenCV) repo.
- Adopt the basic GUI template.

- Add additional widgets to set the parameters.



## Dependencies and System Requirement
- C++ 11 (tested using GCC 9.3.0)
- OpenCV (tested using OpenCV 4.2)
- OpenMV 
- QtCreator 5.0.1
- Qt 5.15.2

Tested on Ubuntu 20.04 LTS
