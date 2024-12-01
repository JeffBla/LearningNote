import sys
from PyQt5.QtWidgets import (QApplication, QMainWindow, QWidget, QPushButton,
                             QVBoxLayout, QHBoxLayout, QLabel, QLineEdit,
                             QGroupBox, QFileDialog)
from PyQt5.QtCore import Qt
import cv2 as cv

from ImgProcess_1 import ColorSeparation, ColorTransformation, ColorExtraction
from ImgSmooth_2 import GuassianFilter, BilateralFilter, MedianFilter
from EdgeDetection_3 import SobelX, SobelY, Combination_Thres, GradientAngle
from Transforms_4 import transform


class ImageProcessingUI(QMainWindow):

    def __init__(self):
        super().__init__()
        self.img1 = None
        self.img2 = None

        self.init_ui()

    def init_ui(self):
        # Create main widget and layout
        main_widget = QWidget()
        self.setCentralWidget(main_widget)
        main_layout = QHBoxLayout(
            main_widget)  # Changed to HBoxLayout for better spacing

        # Create three columns
        load_image_column = QVBoxLayout()
        left_column = QVBoxLayout()
        right_column = QVBoxLayout()

        # Set stretch factors for the layouts
        main_layout.setStretch(0, 1)  # Load image column
        main_layout.setStretch(1, 2)  # Left column
        main_layout.setStretch(2, 2)  # Right column

        # Load Image buttons
        load_image_group = QGroupBox("Load Image")
        load_image_layout = QVBoxLayout()
        btn_load_image1 = QPushButton("Load Image 1")
        btn_load_image1.clicked.connect(self.load_image1)
        btn_load_image2 = QPushButton("Load Image 2")
        btn_load_image2.clicked.connect(self.load_image2)

        load_image_layout.addWidget(btn_load_image1)
        load_image_layout.addWidget(btn_load_image2)
        load_image_layout.addStretch()
        load_image_group.setLayout(load_image_layout)
        load_image_column.addWidget(load_image_group)
        load_image_column.addStretch()

        # 1. Image Processing group
        image_processing_group = QGroupBox("1. Image Processing")
        image_processing_layout = QVBoxLayout()
        btn_color_separation = QPushButton("1.1 Color Separation")
        btn_color_separation.clicked.connect(
            lambda: ColorSeparation(self.img1))
        btn_color_transformation = QPushButton("1.2 Color Transformation")
        btn_color_transformation.clicked.connect(
            lambda: ColorTransformation(self.img1))
        btn_color_extraction = QPushButton("1.3 Color Extraction")
        btn_color_extraction.clicked.connect(
            lambda: ColorExtraction(self.img1))

        image_processing_layout.addWidget(btn_color_separation)
        image_processing_layout.addWidget(btn_color_transformation)
        image_processing_layout.addWidget(btn_color_extraction)
        image_processing_layout.addStretch()
        image_processing_group.setLayout(image_processing_layout)

        # 2. Image Smoothing group
        image_smoothing_group = QGroupBox("2. Image Smoothing")
        image_smoothing_layout = QVBoxLayout()
        btn_gaussian = QPushButton("2.1 Gaussian blur")
        btn_gaussian.clicked.connect(lambda: GuassianFilter(self.img1))
        btn_bilateral = QPushButton("2.2 Bilateral filter")
        btn_bilateral.clicked.connect(lambda: BilateralFilter(self.img1))
        btn_median = QPushButton("2.3 Median filter")
        btn_median.clicked.connect(lambda: MedianFilter(self.img2))

        image_smoothing_layout.addWidget(btn_gaussian)
        image_smoothing_layout.addWidget(btn_bilateral)
        image_smoothing_layout.addWidget(btn_median)
        image_smoothing_layout.addStretch()
        image_smoothing_group.setLayout(image_smoothing_layout)

        # 3. Edge Detection group
        edge_detection_group = QGroupBox("3. Edge Detection")
        edge_detection_layout = QVBoxLayout()
        btn_sobel_x = QPushButton("3.1 Sobel X")
        btn_sobel_x.clicked.connect(lambda: SobelX(self.img1))
        btn_sobel_y = QPushButton("3.2 Sobel Y")
        btn_sobel_y.clicked.connect(lambda: SobelY(self.img1))
        btn_combination = QPushButton("3.3 Combination and\nThreshold")
        btn_combination.clicked.connect(lambda: Combination_Thres(self.img1))
        btn_gradient = QPushButton("3.4 Gradient Angle")
        btn_gradient.clicked.connect(lambda: GradientAngle(self.img1))

        edge_detection_layout.addWidget(btn_sobel_x)
        edge_detection_layout.addWidget(btn_sobel_y)
        edge_detection_layout.addWidget(btn_combination)
        edge_detection_layout.addWidget(btn_gradient)
        edge_detection_layout.addStretch()
        edge_detection_group.setLayout(edge_detection_layout)

        # Add groups to left column
        left_column.addWidget(image_processing_group)
        left_column.addWidget(image_smoothing_group)
        left_column.addWidget(edge_detection_group)
        left_column.addStretch()

        # 4. Transforms group
        transforms_group = QGroupBox("4. Transforms")
        transforms_layout = QVBoxLayout()

        # Create grid layout for transform parameters
        params_layout = QVBoxLayout()

        # Rotation
        rotation_layout = QHBoxLayout()
        rotation_label = QLabel("Rotation:")
        rotation_input = QLineEdit()
        rotation_deg = QLabel("deg")
        rotation_layout.addWidget(rotation_label)
        rotation_layout.addWidget(rotation_input)
        rotation_layout.addWidget(rotation_deg)

        # Scaling
        scaling_layout = QHBoxLayout()
        scaling_label = QLabel("Scaling:")
        scaling_input = QLineEdit()
        scaling_layout.addWidget(scaling_label)
        scaling_layout.addWidget(scaling_input)
        scaling_layout.addStretch()

        # Translation X
        tx_layout = QHBoxLayout()
        tx_label = QLabel("Tx:")
        tx_input = QLineEdit()
        tx_pixel = QLabel("pixel")
        tx_layout.addWidget(tx_label)
        tx_layout.addWidget(tx_input)
        tx_layout.addWidget(tx_pixel)

        # Translation Y
        ty_layout = QHBoxLayout()
        ty_label = QLabel("Ty:")
        ty_input = QLineEdit()
        ty_pixel = QLabel("pixel")
        ty_layout.addWidget(ty_label)
        ty_layout.addWidget(ty_input)
        ty_layout.addWidget(ty_pixel)

        # Transform button
        btn_transform = QPushButton("4. Transforms")
        btn_transform.clicked.connect(lambda: transform(
            self.img1, float(rotation_input.text()), float(scaling_input.text(
            )), float(tx_input.text()), float(ty_input.text())))
        # Add all layouts to transforms group
        params_layout.addLayout(rotation_layout)
        params_layout.addLayout(scaling_layout)
        params_layout.addLayout(tx_layout)
        params_layout.addLayout(ty_layout)
        params_layout.addWidget(btn_transform)
        params_layout.addStretch()

        transforms_layout.addLayout(params_layout)
        transforms_group.setLayout(transforms_layout)

        # Add transforms group to right column
        right_column.addWidget(transforms_group)
        right_column.addStretch()

        # Add columns to main layout
        main_layout.addLayout(load_image_column)
        main_layout.addLayout(left_column)
        main_layout.addLayout(right_column)

        # Window settings
        self.setWindowTitle('Hw1')
        self.setGeometry(100, 100, 800, 600)
        self.setMinimumSize(600, 400)

    def load_image(self):
        filename, _ = QFileDialog.getOpenFileName(
            self, 'Open file', filter="Image files (*.jpg *.png)")
        return filename

    def load_image1(self):
        filename = self.load_image()
        self.img1 = cv.imread(filename)

    def load_image2(self):
        filename = self.load_image()
        self.img2 = cv.imread(filename)


def main():
    app = QApplication(sys.argv)
    window = ImageProcessingUI()
    window.show()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
