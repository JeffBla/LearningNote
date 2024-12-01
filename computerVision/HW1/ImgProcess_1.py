import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt


def ColorSeparation(img):
    b, g, r = cv.split(img)
    b_image = cv.merge((b, np.zeros_like(b), np.zeros_like(b)))
    g_image = cv.merge((np.zeros_like(g), g, np.zeros_like(g)))
    r_image = cv.merge((np.zeros_like(r), np.zeros_like(r), r))
    cv.imshow("Blue", b_image)
    cv.imshow("Green", g_image)
    cv.imshow("Red", r_image)
    cv.waitKey(0)
    cv.destroyAllWindows()
    return b_image, g_image, r_image


def ColorTransformation(img):
    # Q1
    cv_gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    # Q2
    avg_gray = np.mean(img, axis=2).astype(np.uint8)
    cv.imshow("cv_gray", cv_gray)
    cv.imshow("avg_gray", avg_gray)
    cv.waitKey(0)
    cv.destroyAllWindows()
    return cv_gray, avg_gray


def ColorExtraction(img):
    hsv = cv.cvtColor(img, cv.COLOR_BGR2HSV)
    lower = np.array([18, 0, 25])
    upper = np.array([85, 255, 255])
    mask = cv.inRange(hsv, lower, upper)
    not_mask = cv.bitwise_not(mask)
    img = cv.bitwise_and(img, img, mask=not_mask)
    cv.imshow("mask", mask)
    cv.imshow("img", img)
    cv.waitKey(0)
    cv.destroyAllWindows()
    return mask, img


if __name__ == "__main__":
    img_file = r"Dataset_OpenCvDl_Hw1\Q1_image\rgb.jpg"
    img = cv.imread(img_file)
    b, g, r = ColorSeparation(img)
    cv_gray, avg_gray = ColorTransformation(img)
    mask, res_img = ColorExtraction(img)
