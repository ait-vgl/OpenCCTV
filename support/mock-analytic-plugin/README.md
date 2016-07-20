# Template of OpenCCTV Video Analytics
This eclipse C++ project shows the steps to follow in order to develop OpenCCTV video analytic plugin (linux shared library .so).

###### Setup shared library project in eclipse
1. eclipse New C++ Project, Shared Library (Empty Project, Linux GCC).
2. Project Properties, C/C++ Build, Settings, GCC C++ Compiler, Miscellaneous, set Position Independent Code (-fPIC).
3. GCC C++ Linker, Library search path (-L) `/usr/local/lib`, Libraries (-l) set at least `opencv_core`.
4. for the project, create New Source Folder (src).
5. in src, create a New Class (YOUR_ANALYTIC.hpp .cpp).
6. Copy `MockAnalyticPlugin/src/analytic` directory into src.
7. Implement YOUR_ANALYTIC Class.

###### Implementation
1. Check MockAnalytic.hpp and .cpp
2. YOUR_ANALYTIC Class must extend `analytic::api::Analytic` Class.
3. Implement init and process functions in YOUR_ANALYTIC.
4. init = initialization. Do init of YOUR_ANALYTIC and return true if init is successful, otherwise return false.
5. Follow the given steps given in MockAnalytic::process function.

###### Steps to follow inside the while loop of the process function
1. Pop a image (`analytic::api::Image_t`) from input queue (`pInputQueue`).
2. Clone the matImage (`cv::Mat`) of that input image into `cv::Mat matToBeProcessed`.
3. Release the matImage (`cv::Mat`) of input image. Please do it, otherwise it will be stored in the memory forever. Finally your analytic will be crashed with memory overflow.
4. Use cloned `matToBeProcessed` for your analysis.
5. After your process/analysis is done, set output to the same popped image (`analytic::api::Image_t`). Set `image.bGenerateAnalyticEvent` and `image.sCustomTextResult`.
6. Push that image (`analytic::api::Image_t`) into output queue (`pOutputQueue`).

###### create() and destroy()
- Implement `extern "C" YOUR_ANALYTIC* create()` and `extern "C" void destroy(YOUR_ANALYTIC* p)`.
- Check `MockAnalyticPlugin/src/MockAnalytic.hpp`.

###### Further info
- Within init() we can access any file on the Analytic directory as path to the Analytic directory will be passed as a input param of the function. Check `MockAnalytic::init()` implementation.
- Better use OpenCV 2.4.9 with all includes in `/usr/local/include` and all libraries in `/usr/local/lib`.
- All available opencv libraries can be found in MockAnalyticPlugin Project Properties, C/C++ Build, Settings, GCC C++ Linker, Libraries (-l).
