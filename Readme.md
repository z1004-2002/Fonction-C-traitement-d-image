# README: Image Processing Program

## Author:
This code was made by `ZOGNING TEDONGMOUO Franc Abel` and `KWABOU TCHINDA Nelda`

## Introduction
This program provides a variety of image processing functionalities for PGM (Portable Graymap) images. Users can perform operations such as image addition, subtraction, contrast enhancement, histogram equalization, zooming, filtering, and edge detection. Results are saved as new PGM files.

## Compilation and Execution

### Compilation
Ensure you have a C compiler (e.g., `gcc`) installed on your system. Compile the program as follows:
```bash
gcc -o image_processor main.c -lm
```

### Execution
Run the program with the following command:
```bash
./image_processor <command> <input_image> [<parameter>]
```

## Commands and Parameters
Below is a detailed description of each command, the expected input, and any additional parameters.

### 1. **Basic Operations**
- **`addition`**: Adds two images.
  ```bash
  ./image_processor addition image1.pgm image2.pgm
  ```
  Output: `somme_img.pgm`

- **`soustraction`**: Subtracts the second image from the first.
  ```bash
  ./image_processor soustraction image1.pgm image2.pgm
  ```
  Output: `diff_img.pgm`

### 2. **Image Enhancements**
- **`contrast`**: Enhances the contrast of an image.
  ```bash
  ./image_processor contrast input_image.pgm
  ```
  Output: `contrast_img.pgm`

- **`eq_histogramme`**: Equalizes the histogram of an image.
  ```bash
  ./image_processor eq_histogramme input_image.pgm
  ```
  Output: `eq_hist_img.pgm`

### 3. **Zooming**
- **`zoom_in`**: Zooms into an image.
  ```bash
  ./image_processor zoom_in input_image.pgm
  ```
  Output: `zoom_in_img.pgm`

- **`zoom_out`**: Zooms out of an image.
  ```bash
  ./image_processor zoom_out input_image.pgm
  ```
  Output: `zoom_out_img.pgm`

### 4. **Thresholding**
- **`seuillage`**: Applies thresholding to an image.
  ```bash
  ./image_processor seuillage input_image.pgm <threshold>
  ```
  Example:
  ```bash
  ./image_processor seuillage input_image.pgm 128
  ```
  Output: `binaire_img.pgm`

- **`otsu`**: Applies Otsu's binarization method to an image.
  ```bash
  ./image_processor otsu input_image.pgm
  ```
  Output: `otsu_img.pgm`

### 5. **Filtering**
- **`moyenneur`**: Applies a mean filter to the image.
  ```bash
  ./image_processor moyenneur input_image.pgm
  ```
  Output: `moyenneur_img.pgm`

- **`gaussien`**: Applies a Gaussian filter to the image.
  ```bash
  ./image_processor gaussien input_image.pgm
  ```
  Output: `gaussien_img.pgm`

### 6. **Edge Detection**
- **`robert`**: Applies the Robert edge detection filter.
  ```bash
  ./image_processor robert input_image.pgm
  ```
  Output: `robert_img.pgm`

- **`prewitt`**: Applies the Prewitt edge detection filter.
  ```bash
  ./image_processor prewitt input_image.pgm
  ```
  Output: `prewitt_img.pgm`

- **`sobel`**: Applies the Sobel edge detection filter.
  ```bash
  ./image_processor sobel input_image.pgm
  ```
  Output: `sobel_img.pgm`

- **`laplace`**: Applies the Laplacian edge detection filter.
  ```bash
  ./image_processor laplace input_image.pgm
  ```
  Output: `laplace_img.pgm`

### 7. **Edge Detection with Thresholding**
- **`robert_seuil`**: Applies Robert edge detection with a threshold.
  ```bash
  ./image_processor robert_seuil input_image.pgm <threshold>
  ```
  Output: `robert_seuil_img.pgm`

- **`prewitt_seuil`**: Applies Prewitt edge detection with a threshold.
  ```bash
  ./image_processor prewitt_seuil input_image.pgm <threshold>
  ```
  Output: `prewitt_seuil_img.pgm`

- **`sobel_seuil`**: Applies Sobel edge detection with a threshold.
  ```bash
  ./image_processor sobel_seuil input_image.pgm <threshold>
  ```
  Output: `sobel_seuil_img.pgm`

- **`laplace_seuil`**: Applies Laplacian edge detection with a threshold.
  ```bash
  ./image_processor laplace_seuil input_image.pgm <threshold>
  ```
  Output: `laplace_seuil_img.pgm`

### 8. **Hough Transform**
- **`hough`**: Applies the Hough transform to the image.
  ```bash
  ./image_processor hough input_image.pgm <threshold>
  ```
  Output: `hough_img.pgm`

### 9. **Luminosity Adjustment**
- **`luminosite`**: Adjusts the brightness of the image.
  ```bash
  ./image_processor luminosite input_image.pgm <delta_luminosity>
  ```
  Example:
  ```bash
  ./image_processor luminosite input_image.pgm 50
  ```
  Output: `lumin_img.pgm`

## Notes
- Ensure all input images are in the PGM format.
- Invalid commands or parameters will result in an error message.
- Output images are saved in the same directory as the program.

## Dependencies
- Standard C libraries (`stdio.h`, `stdlib.h`, `string.h`, `math.h`).
- Custom library functions for PGM image manipulation (e.g., `lecture`, `enregister_pgm`, `filtre_*`).

## License
This project is licensed under the MIT License.
