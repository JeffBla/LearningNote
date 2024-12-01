import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt


def GuassianFilter(img):

    def on_trackbar(val):
        kernel_size = val * 2 + 1
        res = cv.GaussianBlur(img, (kernel_size, kernel_size), 90)
        cv.imshow("Gaussian", res)

    init_kernel_size = (3, 3)
    res = cv.GaussianBlur(img, init_kernel_size, 90)
    cv.imshow("Gaussian", res)
    cv.createTrackbar("m:", "Gaussian", 1, 5, on_trackbar)
    cv.waitKey(0)
    cv.destroyAllWindows()
    return res


def BilateralFilter(img):

    def on_trackbar(val):
        kernel_size = val * 2 + 1
        res = cv.bilateralFilter(img, kernel_size, 90, 90)
        cv.imshow("Bilateral", res)

    res = cv.bilateralFilter(img, 3, 90, 90)
    cv.imshow("Bilateral", res)
    cv.createTrackbar("m:", "Bilateral", 1, 5, on_trackbar)
    cv.waitKey(0)
    cv.destroyAllWindows()
    return res


def MedianFilter(img):

    def on_trackbar(val):
        kernel_size = val * 2 + 1
        res = cv.medianBlur(img, kernel_size)
        cv.imshow("Median", res)

    res = cv.medianBlur(img, 3)
    cv.imshow("Median", res)
    cv.createTrackbar("m:", "Median", 1, 5, on_trackbar)
    cv.waitKey(0)
    cv.destroyAllWindows()
    return res


if __name__ == "__main__":
    img_file = r"Dataset_OpenCvDl_Hw1\Q2_image\image1.jpg"
    img = cv.imread(img_file)
    GuassianFilter(img)
    BilateralFilter(img)
    MedianFilter(img)
