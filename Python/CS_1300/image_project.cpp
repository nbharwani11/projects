
// CSPB 1300 Image Processing Application
// <Naureen Bharwani>
// 1300 Final Project
// Due: Dec 14th, 2020

#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

//***************************************************************************************************//
//                                DO NOT MODIFY THE SECTION BELOW                                    //
//***************************************************************************************************//

// Pixel structure
struct Pixel
{
    // Red, green, blue color values
    int red;
    int green;
    int blue;
};

/**
 * Gets an integer from a binary stream.
 * Helper function for read_image()
 * @param stream the stream
 * @param offset the offset at which to read the integer
 * @return the integer starting at the given offset
 */ 
int get_int(fstream& stream, int offset)
{
    stream.seekg(offset);
    int result = 0;
    int base = 1;
    for (int i = 0; i < 4; i++)
    {   
        result = result + stream.get() * base;
        base = base * 256;
    }
    return result;
}
/**
 * Reads the BMP image specified and returns the resulting image as a vector
 * @param filename BMP image filename
 * @return the image as a vector of vector of Pixels
 */
vector<vector<Pixel>> read_image(string filename)
{
    // Open the binary file
    fstream stream;
    stream.open(filename, ios::in | ios::binary);

    // Get the image properties
    int file_size = get_int(stream, 2);
    int start = get_int(stream, 10);
    int width = get_int(stream, 18);
    int height = get_int(stream, 22);

    // Scan lines must occupy multiples of four bytes
    int scanline_size = width * 3;
    int padding = 0;
    if (scanline_size % 4 != 0)
    {
        padding = 4 - scanline_size % 4;
    }

    // Return empty vector if this is not a valid 24-bit true color image
    if (file_size != start + (scanline_size + padding) * height)
    {
        return {};
    }

    // Create a vector the size of the input image
    vector<vector<Pixel>> image(height, vector<Pixel> (width));

    int pos = start;
    // For each row, starting from the last row to the first
    // Note: BMP files store pixels from bottom to top
    for (int i = height - 1; i >= 0; i--)
    {
        // For each column
        for (int j = 0; j < width; j++)
        {
            // Go to the pixel position
            stream.seekg(pos);

            // Save the pixel values to the image vector
            // Note: BMP files store pixels in blue, green, red order
            image[i][j].blue = stream.get();
            image[i][j].green = stream.get();
            image[i][j].red = stream.get();

            // Advance the position to the next pixel
            pos = pos + 3;
        }

        // Skip the padding at the end of each row
        stream.seekg(padding, ios::cur);
        pos = pos + padding;
    }

    // Close the stream and return the image vector
    stream.close();
    return image;
}

/**
 * Sets a value to the char array starting at the offset using the size
 * specified by the bytes.
 * This is a helper function for write_image()
 * @param arr    Array to set values for
 * @param offset Starting index offset
 * @param bytes  Number of bytes to set
 * @param value  Value to set
 * @return nothing
 */
void set_bytes(unsigned char arr[], int offset, int bytes, int value)
{
    for (int i = 0; i < bytes; i++)
    {
        arr[offset+i] = (unsigned char)(value>>(i*8));
    }
}

/**
 * Write the input image to a BMP file name specified
 * @param filename The BMP file name to save the image to
 * @param image    The input image to save
 * @return True if successful and false otherwise
 */
bool write_image(string filename, const vector<vector<Pixel>>& image)
{
    // Get the image width and height in pixels
    int width_pixels = image[0].size();
    int height_pixels = image.size();

    // Calculate the width in bytes incorporating padding (4 byte alignment)
    int width_bytes = width_pixels * 3;
    int padding_bytes = 0;
    padding_bytes = (4 - width_bytes % 4) % 4;
    width_bytes = width_bytes + padding_bytes;

    // Pixel array size in bytes, including padding
    int array_bytes = width_bytes * height_pixels;

    // Open a file stream for writing to a binary file
    fstream stream;
    stream.open(filename, ios::out | ios::binary);

    // If there was a problem opening the file, return false
    if (!stream.is_open())
    {
        return false;
    }

    // Create the BMP and DIB Headers
    const int BMP_HEADER_SIZE = 14;
    const int DIB_HEADER_SIZE = 40;
    unsigned char bmp_header[BMP_HEADER_SIZE] = {0};
    unsigned char dib_header[DIB_HEADER_SIZE] = {0};

    // BMP Header
    set_bytes(bmp_header,  0, 1, 'B');              // ID field
    set_bytes(bmp_header,  1, 1, 'M');              // ID field
    set_bytes(bmp_header,  2, 4, BMP_HEADER_SIZE+DIB_HEADER_SIZE+array_bytes); // Size of BMP file
    set_bytes(bmp_header,  6, 2, 0);                // Reserved
    set_bytes(bmp_header,  8, 2, 0);                // Reserved
    set_bytes(bmp_header, 10, 4, BMP_HEADER_SIZE+DIB_HEADER_SIZE); // Pixel array offset

    // DIB Header
    set_bytes(dib_header,  0, 4, DIB_HEADER_SIZE);  // DIB header size
    set_bytes(dib_header,  4, 4, width_pixels);     // Width of bitmap in pixels
    set_bytes(dib_header,  8, 4, height_pixels);    // Height of bitmap in pixels
    set_bytes(dib_header, 12, 2, 1);                // Number of color planes
    set_bytes(dib_header, 14, 2, 24);               // Number of bits per pixel
    set_bytes(dib_header, 16, 4, 0);                // Compression method (0=BI_RGB)
    set_bytes(dib_header, 20, 4, array_bytes);      // Size of raw bitmap data (including padding)                     
    set_bytes(dib_header, 24, 4, 2835);             // Print resolution of image (2835 pixels/meter)
    set_bytes(dib_header, 28, 4, 2835);             // Print resolution of image (2835 pixels/meter)
    set_bytes(dib_header, 32, 4, 0);                // Number of colors in palette
    set_bytes(dib_header, 36, 4, 0);                // Number of important colors
    
    // Write the BMP and DIB Headers to the file
    stream.write((char*)bmp_header, sizeof(bmp_header));
    stream.write((char*)dib_header, sizeof(dib_header));

    // Initialize pixel and padding
    unsigned char pixel[3] = {0};
    unsigned char padding[3] = {0};

    // Pixel Array (Left to right, bottom to top, with padding)
    for (int h = height_pixels - 1; h >= 0; h--)
    {
        for (int w = 0; w < width_pixels; w++)
        {
            // Write the pixel (Blue, Green, Red)
            pixel[0] = image[h][w].blue;
            pixel[1] = image[h][w].green;
            pixel[2] = image[h][w].red;
            stream.write((char*)pixel, 3);
        }
        // Write the padding bytes
        stream.write((char *)padding, padding_bytes);
    }

    // Close the stream and return true
    stream.close();
    return true;
}

//***************************************************************************************************//
//                                DO NOT MODIFY THE SECTION ABOVE                                    //
//***************************************************************************************************//

// YOUR FUNCTION DEFINITIONS HERE

/* For each of the image processing functions below, you will be:
    1. taking in the original image as a parameter, 
    2. creating and initializing a new 2-dimensional vector to store the new image data,
    3. iterating through the original image,
    4. performing the requested image processing operation on the pixel values,
    5. saving the modified pixel values to the new vector,
    6. and returning the new vector */


vector<vector<Pixel>> process_1(const vector<vector<Pixel>>& image)
{
    
    // process_1 - Adds vignette effect - dark corners
    // test function
    // retrieve the size of our image
    int rows = image.size();
    int columns = image[0].size();
       
    // define a new 2-dim vector of Pixels new_img
    vector<vector<Pixel>> new_img(rows, vector<Pixel> (columns));
    
    //iterate through image
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            // need to get pixel location at [row][col]
            int red = image[row][col].red;  // Gets the red color for the Pixel located at index row, col
            int green = image[row][col].green;  // Gets the green color for the Pixel located at index row, col
            int blue = image[row][col].blue;  // Gets the blue color for the Pixel located at index row, col
                
            // find the distance to the center
            // python --> distance = math.sqrt((col - width / 2)**2 + (row - height / 2)**2)
            double distance = sqrt(pow(col - columns / 2, 2) + pow(row - rows / 2, 2));
              
            
            // calculate scaling factor
            double scaling_factor = (rows - distance) / rows;   
                
            // new color at each pixel location [row][col]
            int newred = red * scaling_factor;
            int newgreen = green * scaling_factor;
            int newblue = blue * scaling_factor;
            
                
            // set new pixel to new color values
            // python --> newpixel = image.Pixel(newred, newgreen, newblue)
            // python --> newimg.setPixel(col, row, newpixel)
            new_img[row][col].red = newred;
            new_img[row][col].green = newgreen;
            new_img[row][col].blue = newblue;
        }
    }
    
    return new_img;
}


vector<vector<Pixel>> process_2(const vector<vector<Pixel>>& image, double scaling_factor)
{
    //process_2 - Adds Clarendon effect to image (darks darker and lights lighter) by a scaling factor
    // test function
    // retrieve the size of our image
    int rows = image.size();
    int columns = image[0].size();
        
    // define a new 2-dim vector of Pixels new_img
    vector<vector<Pixel>> new_img(rows, vector<Pixel> (columns));
    
    //iterate through image
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            // need to get pixel location at [row][col]
            int red = image[row][col].red;  // Gets the red color for the Pixel located at index row, col
            int green = image[row][col].green;  // Gets the green color for the Pixel located at index row, col
            int blue = image[row][col].blue;  // Gets the blue color for the Pixel located at index row, col
                
            // average values together to determine color of pixel
            int average_value = (red + green + blue) / 3; 
             
            // new color at each pixel location [row][col]
            if (average_value >= 170)
            {
                int newred = (255 - (255 - red) * scaling_factor);
                int newgreen = (255 - (255 - green) * scaling_factor);
                int newblue =  (255 - (255 - blue) * scaling_factor);
                
                // set new pixel to new color values
                new_img[row][col].red = newred;
                new_img[row][col].green = newgreen;
                new_img[row][col].blue = newblue;
            }
            
            else if (average_value < 90)
            {
                int newred = red * scaling_factor;
                int newgreen = green * scaling_factor;
                int newblue =  blue * scaling_factor;
                
                // set new pixel to new color values
                new_img[row][col].red = newred;
                new_img[row][col].green = newgreen;
                new_img[row][col].blue = newblue;
            }        
            
            else
            {
                int newred = red;
                int newgreen = green;
                int newblue =  blue;
                
                // set new pixel to new color values
                new_img[row][col].red = newred;
                new_img[row][col].green = newgreen;
                new_img[row][col].blue = newblue;
            }
                
        }
    } 
    
    return new_img;
} 


vector<vector<Pixel>> process_3(const vector<vector<Pixel>>& image)
{
    //process_3 - Grayscale image
    // test function
    // retrieve the size of our image
    int rows = image.size();
    int columns = image[0].size();
        
    // define a new 2-dim vector of Pixels new_img
    vector<vector<Pixel>> new_img(rows, vector<Pixel> (columns));
    
    //iterate through image
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            // need to get pixel location at [row][col]
            int red = image[row][col].red;  // Gets the red color for the Pixel located at index row, col
            int green = image[row][col].green;  // Gets the green color for the Pixel located at index row, col
            int blue = image[row][col].blue;  // Gets the blue color for the Pixel located at index row, col
                
            // average those values together to determine new color of pixel
            int gray_value = (red + green + blue) / 3; 
             
            // new color at each pixel location [row][col]
            int newred = gray_value;
            int newgreen = gray_value;
            int newblue = gray_value;
                
            // set new pixel to new color values
            new_img[row][col].red = newred;
            new_img[row][col].green = newgreen;
            new_img[row][col].blue = newblue;
        }
    } 
    
    return new_img;
}


vector<vector<Pixel>> process_4(const vector<vector<Pixel>>& image)
{
    //process_4 - Rotates image by 90 degrees clockwise (not counter-clockwise)
    // test function
    // retrieve the size of our image
    int rows = image.size();
    int columns = image[0].size();
        
    // define a new 2-dim vector of Pixels new_img
    vector<vector<Pixel>> new_img(columns, vector<Pixel> (rows));
    int new_rows = new_img.size();
    int new_columns = new_img[0].size();
    
    //iterate through image
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            // need to get pixel location at [row][col]
            int red = image[row][col].red;  // Gets the red color for the Pixel located at index row, col
            int green = image[row][col].green;  // Gets the green color for the Pixel located at index row, col
            int blue = image[row][col].blue;  // Gets the blue color for the Pixel located at index row, col
                
            // python --> p = img.getPixel(col, row)
            // python --> newimg.setPixel( (height-1) - row, col, p)   
            // set new pixel to new color values
            // change orientation to flip image 90 degrees
            new_img[col][(rows - 1) - row].red = red;
            new_img[col][(rows - 1) - row].green = green;
            new_img[col][(rows - 1) - row].blue = blue;
        }
    } 
    
    return new_img;
}


vector<vector<Pixel>> process_5(const vector<vector<Pixel>>& image, int number)
{
    //process_5 - Rotates image by a specified number of multiples of 90 degrees clockwise
    // 90 degrees times num of rotations wanted
    int angle = (number * 90);
    // if rotate 4 times then return original image
    if (angle % 360 == 0)
    { 
        return image;
    }
    
    // if rotate once pass to process_4 once
    else if (angle % 360 == 90)
    {
        return process_4(image);
    }
    
    // if rotate twice pass to process_4 twice
    else if (angle % 360 == 180)
    {
        return process_4(process_4(image));
    }
    
    // if rotate 3 times pass to process_4 3 times
    else
    {
        return process_4(process_4(process_4(image)));
    }

}


vector<vector<Pixel>> process_6(const vector<vector<Pixel>>& image, int x_scale, int y_scale)
{
    //process_6 - Enlarges the image in the x and y direction
    // test function
    // retrieve the size of our image
    int rows = image.size();
    int columns = image[0].size();
        
    // define a new 2-dim vector of Pixels new_img
    vector<vector<Pixel>> new_img(y_scale * rows, vector<Pixel> (x_scale * columns));
    // need new size of new image to iterate through
    int new_rows = new_img.size();
    int new_columns = new_img[0].size();
    
    //iterate through image
    for (int row = 0; row < new_rows; row++)
    {
        for (int col = 0; col < new_columns; col++)
        {
            // need to get pixel location at [row][col]
            int red = image[row/y_scale][col/x_scale].red;  // Gets the red color for the Pixel located at index row, col
            int green = image[row/y_scale][col/x_scale].green;  // Gets the green color for the Pixel located at index row, col
            int blue = image[row/y_scale][col/x_scale].blue;  // Gets the blue color for the Pixel located at index row, col
                
            // python --> newpixel = image.Pixel(newred, newgreen, newblue)
            // python --> newimg.setPixel(col, row, newpixel)
            // set new pixel to new color values
            new_img[row][col].red = red;
            new_img[row][col].green = green;
            new_img[row][col].blue = blue;
        }
    }
    
    return new_img;
    
}


vector<vector<Pixel>> process_7(const vector<vector<Pixel>>& image)
{
    //process_7 - Convert image to high contrast (black and white only)
    // test function
    // retrieve the size of our image
    int rows = image.size();
    int columns = image[0].size();
        
    // define a new 2-dim vector of Pixels new_img
    vector<vector<Pixel>> new_img(rows, vector<Pixel> (columns));
    
    //iterate through image
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            // need to get pixel location at [row][col]
            int red = image[row][col].red;  // Gets the red color for the Pixel located at index row, col
            int green = image[row][col].green;  // Gets the green color for the Pixel located at index row, col
            int blue = image[row][col].blue;  // Gets the blue color for the Pixel located at index row, col
               
            // average those values to get the gray value
            int gray_value = (red + green + blue) / 3;
            
            // new color at each pixel location [row][col]
            // either black or white
            if (gray_value >= 255/2)
            {
                int newred = 255;
                int newgreen = 255;
                int newblue = 255;
                    
                // python --> newpixel = image.Pixel(newred, newgreen, newblue)
                // python --> newimg.setPixel(col, row, newpixel)
                // set new pixel to new color values
                new_img[row][col].red = newred;
                new_img[row][col].green = newgreen;
                new_img[row][col].blue = newblue;
            }
            
            else
            {
                int newred = 0;
                int newgreen = 0;
                int newblue = 0;
                    
                // python --> newpixel = image.Pixel(newred, newgreen, newblue)
                // python --> newimg.setPixel(col, row, newpixel)
                // set new pixel to new color values
                new_img[row][col].red = newred;
                new_img[row][col].green = newgreen;
                new_img[row][col].blue = newblue;
            }
        }
    }
    
    return new_img;  
}


vector<vector<Pixel>> process_8(const vector<vector<Pixel>>& image, double scaling_factor)
{   
    //process_8 - Lightens image by a scaling factor
    // test function
    // retrieve the size of our image
    int rows = image.size();
    int columns = image[0].size();
        
    // define a new 2-dim vector of Pixels new_img
    vector<vector<Pixel>> new_img(rows, vector<Pixel> (columns));
    
    //iterate through image
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            // need to get pixel location at [row][col]
            int red = image[row][col].red;  // Gets the red color for the Pixel located at index row, col
            int green = image[row][col].green;  // Gets the green color for the Pixel located at index row, col
            int blue = image[row][col].blue;  // Gets the blue color for the Pixel located at index row, col
                
            // new color at each pixel location [row][col]
            int newred = (255 - (255 - red) * scaling_factor);
            int newgreen = (255 - (255 - green) * scaling_factor);
            int newblue = (255 - (255 - blue) * scaling_factor);
            
                
            // python --> newpixel = image.Pixel(newred, newgreen, newblue)
            // python --> newimg.setPixel(col, row, newpixel)
            // set new pixel to new color values
            new_img[row][col].red = newred;
            new_img[row][col].green = newgreen;
            new_img[row][col].blue = newblue;
        }
    }
    
    return new_img;
    
}


vector<vector<Pixel>> process_9(const vector<vector<Pixel>>& image, double scaling_factor)
{   
    //process_9 - Darkens image by a scaling factor
    // test function
    // retrieve the size of our image
    int rows = image.size();
    int columns = image[0].size();
        
    // define a new 2-dim vector of Pixels new_img
    vector<vector<Pixel>> new_img(rows, vector<Pixel> (columns));
    
    //iterate through image
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            // need to get pixel location at [row][col]
            int red = image[row][col].red;  // Gets the red color for the Pixel located at index row, col
            int green = image[row][col].green;  // Gets the green color for the Pixel located at index row, col
            int blue = image[row][col].blue;  // Gets the blue color for the Pixel located at index row, col
                
            // new color at each pixel location [row][col]
            int newred = red * scaling_factor;
            int newgreen = green * scaling_factor;
            int newblue = blue * scaling_factor;
            
            // python --> newpixel = image.Pixel(newred, newgreen, newblue)
            // python --> newimg.setPixel(col, row, newpixel)
            // set new pixel to new color values
            new_img[row][col].red = newred;
            new_img[row][col].green = newgreen;
            new_img[row][col].blue = newblue;
        }
    }
    
    return new_img;
    
    
}


vector<vector<Pixel>> process_10(const vector<vector<Pixel>>& image)
{
    //process_10 - Converts image to only black, white, red, blue, and green
    // test function
    // retrieve the size of our image
    int rows = image.size();
    int columns = image[0].size();
        
    // define a new 2-dim vector of Pixels new_img
    vector<vector<Pixel>> new_img(rows, vector<Pixel> (columns));
    
    //iterate through image
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            // need to get pixel location at [row][col]
            int red = image[row][col].red;  // Gets the red color for the Pixel located at index row, col
            int green = image[row][col].green;  // Gets the green color for the Pixel located at index row, col
            int blue = image[row][col].blue;  // Gets the blue color for the Pixel located at index row, col
            
            // max color between the three
            // comparing pixel ints to find max color
            int max_color = 0;
            if (red >= green)
            { max_color = red; }
            
            else if (green >= blue)
            { max_color = green; }
            
            else if (blue >= red)
            { max_color = blue; }
            
            
            if (red + green + blue >= 550)
            {
                int newred = 255;
                int newgreen = 255;
                int newblue = 255;
                
                // python --> newpixel = image.Pixel(newred, newgreen, newblue)
                // python --> newimg.setPixel(col, row, newpixel)
                // set new pixel to new color values
                new_img[row][col].red = newred;
                new_img[row][col].green = newgreen;
                new_img[row][col].blue = newblue;
            }
            
            else if (red + green + blue <= 150)
            {
                int newred = 0;
                int newgreen = 0;
                int newblue = 0;
                
                // python --> newpixel = image.Pixel(newred, newgreen, newblue)
                // python --> newimg.setPixel(col, row, newpixel)
                // set new pixel to new color values
                new_img[row][col].red = newred;
                new_img[row][col].green = newgreen;
                new_img[row][col].blue = newblue;
            }
                    
            else if (max_color == red)
            {
                int newred = 255;
                int newgreen = 0;
                int newblue = 0;
                
                // python --> newpixel = image.Pixel(newred, newgreen, newblue)
                // python --> newimg.setPixel(col, row, newpixel)
                // set new pixel to new color values
                new_img[row][col].red = newred;
                new_img[row][col].green = newgreen;
                new_img[row][col].blue = newblue;
            }        
            
            else if (max_color == green)
            {
                int newred = 0;
                int newgreen = 255;
                int newblue = 0;
                
                // python --> newpixel = image.Pixel(newred, newgreen, newblue)
                // python --> newimg.setPixel(col, row, newpixel)
                // set new pixel to new color values
                new_img[row][col].red = newred;
                new_img[row][col].green = newgreen;
                new_img[row][col].blue = newblue;
            }
            
            else
            {
                int newred = 0;
                int newgreen = 0;
                int newblue = 255;
                
                // python --> newpixel = image.Pixel(newred, newgreen, newblue)
                // python --> newimg.setPixel(col, row, newpixel)
                // set new pixel to new color values
                new_img[row][col].red = newred;
                new_img[row][col].green = newgreen;
                new_img[row][col].blue = newblue;
            }
        }
    }
    return new_img;
}

// -------------------------------------------------------------------------------- \\


int main()
{
    // YOUR CODE HERE
    
    // read image in
    string image_name;
    
    // declare variables to be used before calling functions
    double scaling_factor = 0.3;
    int number = 2;
    int x_scale = 2;
    int y_scale = 3;
    
    // display first part of menu to capture user input for image name
    
    cout << "CSPB 1300 Image Processing Application" << endl;
    cout << "Please enter an image filename: ";
    cin >> image_name;
    vector<vector<Pixel>> image = read_image(image_name);
    
    
    bool done = false;
    while (!done)
    {   
        // display menu selection 
        cout << "IMAGE PROCESSING MENU" << endl;
        cout << "0) Change image (current: " << image_name << ")" << endl;
        cout << "1) Vignette" << endl;
        cout << "2) Clarendon" << endl;
        cout << "3) Grayscale" << endl;
        cout << "4) Rotate 90 degrees" << endl;
        cout << "5) Rotate multiple 90 degrees" << endl;
        cout << "6) Enlarge" << endl;
        cout << "7) High contrast" << endl;
        cout << "8) Lighten" << endl;
        cout << "9) Darken" << endl;
        cout << "10) Black, white, red, green, blue" << endl;
        cout << "Enter a single menu selection (Q to quit): ";
        int menu_selection;
        cin >> menu_selection;
        
        // declare output filename before calling functions
        string output_filename;

        // if user input fails then will QqUIT PROGRAM
        if (cin.fail())
        {
            done = true;
            cout << "Thank you for using my program!" << endl;
            cout << "Quitting..." << endl;
            return 0;
        }
        
        else
        {
        
            if (menu_selection > 10 || menu_selection < 0)
            {
                cout << "Please enter a valid menu selection number ";
            }    
            
            // 0 selected from menu selection
            if (menu_selection == 0)
            {
                cout << "Change image selected" << endl;
                cout << "Please enter new input BMP filename: ";
                cin >> image_name;
                cout << "Successfully changed input image!" << endl; 
            }
    
            // 1 selected from menu selection
            else if(menu_selection == 1)
            {
                cout << "Vignette selected" << endl;
                cout << "Please enter output BMP filename: ";
                cin >> output_filename;
                vector<vector<Pixel>> process1_result = process_1(image);
                write_image(output_filename, process1_result);
                cout << "Successfully applied vignette!" << endl;
            }
    
            // 2 selected from menu selection
            else if(menu_selection == 2)
            {
                cout << "Clarendon selected" << endl;
                cout << "Please enter output BMP filename: ";
                cin >> output_filename;
                cout << "Please enter scaling factor: ";
                cin >> scaling_factor;
                vector<vector<Pixel>> process2_result = process_2(image, scaling_factor);
                write_image(output_filename, process2_result);
                cout << "Successfully applied clarendon!" << endl;
            }
    
            // 3 selected from menu selection
            else if(menu_selection == 3)
            {
                cout << "Grayscale selected" << endl;
                cout << "Please enter output BMP filename: ";
                cin >> output_filename;
                vector<vector<Pixel>> process3_result = process_3(image);
                write_image(output_filename, process3_result);
                cout << "Successfully applied grayscale!" << endl;    
            }
    
            // 4 selected from menu selection
            else if(menu_selection == 4)
            {
                cout << "Rotate 90 degrees selected" << endl;
                cout << "Please enter output BMP filename: ";
                cin >> output_filename;
                vector<vector<Pixel>> process4_result = process_4(image);
                write_image(output_filename, process4_result);
                cout << "Successfully applied 90 degree rotation!" << endl;
            }
    
            // 5 selected from menu selection
            else if(menu_selection == 5)
            {
                cout << "Rotate multiple 90 degrees selected" << endl;
                cout << "Please enter output BMP filename: ";
                cin >> output_filename;
                cout << "Please enter number of 90 degree rotations: ";
                cin >> number;
                vector<vector<Pixel>> process5_result = process_5(image, number);
                write_image(output_filename, process5_result);
                cout << "Successfully applied multiple 90 degree rotations!" << endl;
            }
    
            // 6 selected from menu selection
            else if(menu_selection == 6)
            {
                cout << "Enlarge selected" << endl;
                cout << "Please enter output BMP filename: ";
                cin >> output_filename;
                cout << "Please enter X scale: ";
                cin >> x_scale;
                cout << "Please Enter Y scale: ";
                cin >> y_scale;
                vector<vector<Pixel>> process6_result = process_6(image, x_scale, y_scale);
                write_image(output_filename, process6_result);
                cout << "Successfully enlarged!" << endl;
            }
    
            // 7 selected from menu selection
            else if(menu_selection == 7)
            {
                cout << "High contrast selected" << endl;
                cout << "Please enter output BMP filename: ";
                cin >> output_filename;
                vector<vector<Pixel>> process7_result = process_7(image);
                write_image(output_filename, process7_result);
                cout << "Successfully applied high contrast!" << endl;
            }

            // 8 selected from menu selection
            else if(menu_selection == 8)
            {
                cout << "Lighten selected" << endl;
                cout << "Please enter output BMP filename: ";
                cin >> output_filename;
                cout << "Please enter scaling factor: ";
                cin >> scaling_factor;
                vector<vector<Pixel>> process8_result = process_8(image, scaling_factor);
                write_image(output_filename, process8_result);
                cout << "Successfully lightened!" << endl;
            }
    
            // 9 selected from menu selection
            else if(menu_selection == 9)
            {
                cout << "Darken selected" << endl;
                cout << "Please enter output BMP filename: ";
                cin >> output_filename;
                cout << "Please enter scaling factor: ";
                cin >> scaling_factor;
                vector<vector<Pixel>> process9_result = process_9(image, scaling_factor);
                write_image(output_filename, process9_result);
                cout << "Successfully darkened!" << endl;
            }
    
            // 10 selected from menu selection
            else if(menu_selection == 10)
            {
                cout << "Black, white, red, green, blue selected" << endl;
                cout << "Please enter output BMP filename: ";
                cin >> output_filename;
                vector<vector<Pixel>> process10_result = process_10(image);
                write_image(output_filename, process10_result);
                cout << "Successfully applied black, white, red, green, blue filter!" << endl;
            }    
        }
    }
  return 0;
}
        
    
    /* Tiny Test Function 

    vector<vector<Pixel>> tiny =
    {
        {{  0,  5, 10},{ 15, 20, 25},{ 30, 35, 40},{ 45, 50, 55}},
        {{ 60, 65, 70},{ 75, 80, 85},{ 90, 95,100},{105,110,115}},
        {{120,125,130},{135,140,145},{150,155,160},{165,170,175}}
    };

    double scaling_factor = 0.3;
    int number = 2;
    int x_scale = 2;
    int y_scale = 3;
    
    vector<vector<Pixel>> process1_result = process_1(image);
    vector<vector<Pixel>> process2_result = process_2(image, scaling_factor);
    vector<vector<Pixel>> process3_result = process_3(image);
    vector<vector<Pixel>> process4_result = process_4(image);
    vector<vector<Pixel>> process5_result = process_5(image, number);
    vector<vector<Pixel>> process6_result = process_6(image, x_scale, y_scale);
    vector<vector<Pixel>> process7_result = process_7(image);
    scaling_factor = 0.5;
    vector<vector<Pixel>> process8_result = process_8(image, scaling_factor);
    vector<vector<Pixel>> process9_result = process_9(image, scaling_factor);
    vector<vector<Pixel>> process10_result = process_10(image);

    
    for (int row = 0; row < result.size(); row++)
    {
        for (int col = 0; col < result[0].size(); col++)
        {
            cout << setw(3) << result[row][col].red << " ";
            cout << setw(3) << result[row][col].green << " ";
            cout << setw(3) << result[row][col].blue << " ";
        }
        cout << endl;
    }
    
    
    // write function to make new image
    // bool write_image(string final_image, const vector<vector<Pixel>>& image);
    
    // process_1 - Adds vignette effect - dark corners
    write_image("process_1.bmp", process1_result);

    //process_2 - Adds Clarendon effect to image (darks darker and lights lighter) by a scaling factor
        // double scaling_factor = 0.8
        // recommended function signature = vector<vector<Pixel>> process_2(const vector<vector<Pixel>>& image, double scaling_factor)
    write_image("process_2.bmp", process2_result);
    
    //process_3 - Grayscale image
        // recommended function signature = vector<vector<Pixel>> process_3(const vector<vector<Pixel>>& image)
    write_image("process_3.bmp", process3_result);
    
    //process_4 - Rotates image by 90 degrees clockwise (not counter-clockwise)
        // recommended function signature = vector<vector<Pixel>> process_4(const vector<vector<Pixel>>& image)
    write_image("process_4.bmp", process4_result);
    
    //process_5 - Rotates image by a specified number of multiples of 90 degrees clockwise
        // recommended function signature = vector<vector<Pixel>> process_5(const vector<vector<Pixel>>& image, int number)
    write_image("process_5.bmp", process5_result);
    
    //process_6 - Enlarges the image in the x and y direction
        // recommended function signature = vector<vector<Pixel>> process_6(const vector<vector<Pixel>>& image, int x_scale, int y_scale)
    write_image("process_6.bmp", process6_result);
    
    //process_7 - Convert image to high contrast (black and white only)
        // recommended function signature = vector<vector<Pixel>> process_7(const vector<vector<Pixel>>& image)
    write_image("process_7.bmp", process7_result);
    
    //process_8 - Lightens image by a scaling factor
        // recommended function signature = vector<vector<Pixel>> process_8(const vector<vector<Pixel>>& image, double scaling_factor)
    write_image("process_8.bmp", process8_result);

    
    //process_9 - Darkens image by a scaling factor
        // recommended function signature = vector<vector<Pixel>> process_9(const vector<vector<Pixel>>& image, double scaling_factor)
    write_image("process_9.bmp", process9_result);

    
    //process_10 - Converts image to only black, white, red, blue, and green
        // recommended function signature = vector<vector<Pixel>> process_10(const vector<vector<Pixel>>& image)
    write_image("process_10.bmp", process10_result); */
    
