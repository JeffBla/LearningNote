import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt


def _SobelX(img):

    def mSobelX(gaussian):
        x_filter = np.array([[+1, 0, -1], [+2, 0, -2], [+1, 0, -1]])
        sobelx = np.zeros_like(gaussian, dtype=np.float64)
        pad_gaussian = np.pad(gaussian, pad_width=1, mode='edge')
        for i in range(gaussian.shape[0]):
            for j in range(gaussian.shape[1]):
                sobelx[i,
                       j] = np.sum(pad_gaussian[i:i + 3, j:j + 3] * x_filter)
        return sobelx  # Return raw gradient values

    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    gaussian = cv.GaussianBlur(gray, (3, 3), 1)
    return mSobelX(gaussian)


def _SobelY(img):

    def mSobelY(gaussian):
        y_filter = np.array([[+1, +2, +1], [0, 0, 0], [-1, -2, -1]])
        sobely = np.zeros_like(gaussian, dtype=np.float64)
        pad_gaussian = np.pad(gaussian, pad_width=1, mode='edge')
        for i in range(gaussian.shape[0]):
            for j in range(gaussian.shape[1]):
                sobely[i,
                       j] = np.sum(pad_gaussian[i:i + 3, j:j + 3] * y_filter)
        return sobely  # Return raw gradient values

    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    gaussian = cv.GaussianBlur(gray, (3, 3), 1)
    return mSobelY(gaussian)


def _combine_sobel(sobelx, sobely):
    return np.sqrt(sobelx**2 + sobely**2)


def SobelX(img):
    sobelx = _SobelX(img)
    # Normalize for display only
    display_sobelx = np.abs(sobelx)
    display_sobelx = np.clip(display_sobelx, 0, 255).astype(np.uint8)

    cv.imshow("SobelX", display_sobelx)
    cv.waitKey(0)
    cv.destroyAllWindows()
    return sobelx  # Return raw gradient values


def SobelY(img):
    sobely = _SobelY(img)
    # Normalize for display only
    display_sobely = np.abs(sobely)
    display_sobely = np.clip(display_sobely, 0, 255).astype(np.uint8)

    cv.imshow("SobelY", display_sobely)
    cv.waitKey(0)
    cv.destroyAllWindows()
    return sobely  # Return raw gradient values


def Combination_Thres(img):
    # Get raw gradient values
    sobelx = _SobelX(img)
    sobely = _SobelY(img)

    sobelx = np.abs(sobelx)
    sobely = np.abs(sobely)

    sobelx = np.clip(sobelx, 0, 255)
    sobely = np.clip(sobely, 0, 255)

    # Compute magnitude
    magnitude = _combine_sobel(sobelx, sobely)

    # Normalize magnitude for display and thresholding
    normalized = cv.normalize(magnitude, None, 0, 255,
                              cv.NORM_MINMAX).astype(np.uint8)

    # Apply thresholds
    _, res_128 = cv.threshold(normalized, 128, 255, cv.THRESH_BINARY)
    _, res_28 = cv.threshold(normalized, 28, 255, cv.THRESH_BINARY)

    # Display results
    cv.imshow("Sobel Magnitude", normalized)
    cv.imshow("Threshold_128", res_128)
    cv.imshow("Threshold_28", res_28)
    cv.waitKey(0)
    cv.destroyAllWindows()

    return normalized, res_128, res_28


def _GradientAngle(sobelx, sobely):
    angle = np.arctan2(sobely, sobelx) * 180 / np.pi
    # Ensure angles are in range [0, 360)
    angle = np.mod(angle + 360, 360)
    return angle


def _create_angle_mask(angle, min_angle, max_angle):
    if min_angle < 0:
        # Handle wrapping around 0 degrees
        return ((angle >= (min_angle + 360)) |
                (angle <= max_angle)).astype(np.uint8) * 255
    else:
        return ((angle >= min_angle) &
                (angle <= max_angle)).astype(np.uint8) * 255


def GradientAngle(img):
    # Calculate gradients
    sobelx = _SobelX(img)
    sobely = _SobelY(img)

    # Calculate magnitude and angle
    sobel = _combine_sobel(sobelx, sobely)
    angle = _GradientAngle(sobelx, sobely)

    # Normalize magnitude for visualization
    # sobel_normalized = cv.normalize(np.abs(sobel), None, 0, 255,
    # cv.NORM_MINMAX).astype(np.uint8)
    sobel_normalized = np.abs(sobel)
    sobel_normalized = np.clip(sobel_normalized, 0, 255).astype(np.uint8)

    # Create masks for specified angle ranges
    # For 170-190 degrees
    mask170_190 = _create_angle_mask(angle, 170, 190)

    # For 260-280 degrees
    mask260_280 = _create_angle_mask(angle, 260, 280)

    # Apply masks to the normalized magnitude
    sobel_170_190 = cv.bitwise_and(sobel_normalized, mask170_190)
    sobel_260_280 = cv.bitwise_and(sobel_normalized, mask260_280)

    # Display results
    cv.imshow("170-190 degrees", sobel_170_190)
    cv.imshow("260-280 degrees", sobel_260_280)
    cv.waitKey(0)
    cv.destroyAllWindows()

    return sobel_170_190, sobel_260_280


if __name__ == "__main__":
    img_file = r"Dataset_OpenCvDl_Hw1\Q3_image\building.jpg"
    img = cv.imread(img_file)
    SobelX(img)
    SobelY(img)
    Combination_Thres(img)
    GradientAngle(img)
