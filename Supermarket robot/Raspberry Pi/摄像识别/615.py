


import numpy as np
import cv2
from matplotlib import pyplot as plt

cv2.ocl.setUseOpenCL(False)
# 读入灰度图像
img1 = cv2.imread('13.png', cv2.IMREAD_GRAYSCALE)
img2 = cv2.imread('14.png', cv2.IMREAD_GRAYSCALE)

# 创建orb特征检测器和描述符
orb = cv2.ORB_create()
kp1, des1 = orb.detectAndCompute(img1, None)
kp2, des2 = orb.detectAndCompute(img2, None)

# knn匹配,返回k个匹配
bf = cv2.BFMatcher(cv2.NORM_L1, crossCheck=False)
matches = bf.knnMatch(des1, des2, k=2)

# 显示图像
img3 = cv2.drawMatchesKnn(img1, kp1, img2, kp2, matches, img2, flags=2)
plt.imshow(img3), plt.show()
