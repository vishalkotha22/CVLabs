import cv2
import matplotlib.pyplot as plt

img = cv2.imread('image w noise.jpg')
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

#image, kernel dimensions
meanFilter = cv2.blur(img, (5, 5))
#plt.imshow(meanFilter)

#image, kernel dimensions, SD along x axis, SD along y axis, border type for edges
gaussianFilter = cv2.GaussianBlur(img, (5,5), 0)
#plt.imshow(gaussianFilter)

#image, kernel size
medianFilter = cv2.medianBlur(img, 5)
#plt.imshow(medianFilter)

#image, neighborhood diameter, sigma of color, sigma of space
bilateralFilter = cv2.bilateralFilter(img, 15, 75, 75)
#plt.imshow(bilateralFilter)

#image, luminance filter strength, chrominance filter strength, template patch size, search window size
nonLocalMeansFilter = cv2.fastNlMeansDenoisingColored(img, 10, 10, 7, 21)
plt.imshow(nonLocalMeansFilter)

plt.show()