#include<sstream>
#include<unistd.h>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<pylon/PylonIncludes.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc, char* argv[])
{
	if( 3!=argc )
	{
		std::cout << "usage: " << argv[0] << " directory_path number_of_images" << std::endl;
		return -1;
	}

	// create frames directory (if not exists)
	struct stat st = {0};
	if( 0>stat(argv[1], &st) )
	{
		mkdir(argv[1], 0700);
	}

    // The exit code of the sample application.
    int exitCode = 0;

    // Automagically call PylonInitialize and PylonTerminate to ensure the pylon runtime system
    // is initialized during the lifetime of this object.
    Pylon::PylonAutoInitTerm autoInitTerm;

    try
    {
		// Create an instant camera object with the camera device found first.
		std::cout << "Creating Camera..." << std::endl;
		Pylon::CInstantCamera camera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());

        // or use a device info object to use a specific camera
		//CDeviceInfo info;
		//info.SetSerialNumber("21694497");
		//CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice(info));
		std::cout << "Camera Created." << std::endl;
        // Print the model name of the camera.
        std::cout << "Using device " << camera.GetDeviceInfo().GetModelName() << std::endl;

        // The parameter MaxNumBuffer can be used to control the count of buffers
        // allocated for grabbing. The default value of this parameter is 10.
        camera.MaxNumBuffer = 10;

		// create pylon image format converter and pylon image
		Pylon::CImageFormatConverter formatConverter;
		formatConverter.OutputPixelFormat= Pylon::PixelType_BGR8packed;
		Pylon::CPylonImage pylonImage;

		// Create an OpenCV image
		cv::Mat openCvImage;

        // Start the grabbing of c_countOfImagesToGrab images.
        // The camera device is parameterized with a default configuration which
        // sets up free-running continuous acquisition.
        camera.StartGrabbing( atoi(argv[2]) );

        // This smart pointer will receive the grab result data.
        Pylon::CGrabResultPtr ptrGrabResult;

		// Create a display window
		cv::namedWindow( "OpenCV Display Window", CV_WINDOW_AUTOSIZE); //FREERATIO

        // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.
		static long frame=0;
        while ( camera.IsGrabbing() && 0x1b!=(char)cv::waitKey(1) )
        {
            // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
            camera.RetrieveResult( 5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);

            // Image grabbed successfully?
            if (ptrGrabResult->GrabSucceeded())
            {
				// Convert the grabbed buffer to pylon image
				formatConverter.Convert(pylonImage, ptrGrabResult);
				// Create an OpenCV image out of pylon image
				openCvImage= cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *) pylonImage.GetBuffer());

				// Display the current image with opencv
				cv::imshow( "OpenCV Display Window", openCvImage);

				char filename[1024];
				snprintf(filename, sizeof(filename), "%s/%05ld.bmp", argv[1], frame++);
				cv::imwrite(filename, openCvImage);
            }
            else
            {
                std::cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << std::endl;
            }
        }
    }
    catch (GenICam::GenericException &e)
    {
        // Error handling.
        std::cerr
			<< "An exception occurred." << std::endl
			<< e.GetDescription() << std::endl;
        exitCode = 1;
    }

    return exitCode;
}
