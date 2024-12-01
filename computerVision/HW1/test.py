import os

os.environ["KMP_DUPLICATE_LIB_OK"] = "TRUE"

from torchvision.datasets import MNIST
import torch
import torchvision.transforms as transforms
import matplotlib.pyplot as plt

# Define a transform to convert the image to tensor and normalize it
transform = transforms.Compose([transforms.ToTensor()])

# Load the MNIST dataset
mnist_data = MNIST(root='./data',
                   train=True,
                   download=True,
                   transform=transform)

# Filter out the images of the number 3
threes = [img for img, label in mnist_data if label == 3]

# Apply a simple direction filter
sobel_filter = torch.tensor([[-1, -1, 1, 1]],
                            dtype=torch.float32).unsqueeze(0).unsqueeze(0)

# Apply the filter to the first image of the number 3
image = threes[0]
filtered_image = torch.nn.functional.conv2d(image.unsqueeze(0), sobel_filter)

# Plot the original and filtered images
plt.subplot(1, 2, 1)
plt.title('Original Image')
plt.imshow(image.squeeze(), cmap='gray')

plt.subplot(1, 2, 2)
plt.title('Filtered Image')
plt.imshow(filtered_image.squeeze().numpy(), cmap='gray')

plt.show()
