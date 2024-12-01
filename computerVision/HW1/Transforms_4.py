import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt


def transform(img, angle=0, scale=1, tx=0, ty=0):
    # Get the image center for scaling and rotation
    center = (240, 200)

    # Rotation matrix for counterclockwise rotation
    Mr = cv.getRotationMatrix2D(center, angle, scale)

    rotated_scaled = cv.warpAffine(img, Mr, img.shape[:2][::-1])

    # Translation matrix
    Mt = np.float64([[1, 0, tx], [0, 1, ty]])

    # Apply translation
    result = cv.warpAffine(rotated_scaled, Mt, img.shape[:2][::-1])

    cv.imshow("Transformed", result)
    cv.waitKey(0)
    cv.destroyAllWindows()
    return result


if __name__ == "__main__":
    img_file = r"Dataset_OpenCvDl_Hw1\Q4_image\burger.png"
    img = cv.imread(img_file)
    transform(img, 30, 0.9, 535, 335)
