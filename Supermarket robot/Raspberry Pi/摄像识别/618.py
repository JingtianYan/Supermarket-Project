
import numpy as np
import cv2
from matplotlib import pyplot as plt

MIN_MATCH_COUNT = 10

# 读入图像
img1 = cv2.imread('11.png', cv2.IMREAD_GRAYSCALE)
img2 = cv2.imread('12.png', cv2.IMREAD_GRAYSCALE)

# 创建sift对象
sift = cv2.xfeatures2d.SIFT_create()
# 查询特征点和描述符
kp1, des1 = sift.detectAndCompute(img1, None)
kp2, des2 = sift.detectAndCompute(img2, None)

FLANN_INDEX_KDTREE = 0
# 创建字典参数
indexParams = dict(algorithm=FLANN_INDEX_KDTREE, trees=5)  # 处理索引
searchParams = dict(checks=50)  # 创建对象,用来指定索引树的遍历次数

flann = cv2.FlannBasedMatcher(indexParams, searchParams)

matches = flann.knnMatch(des1, des2, k=2)

good = []
for m, n in matches:
    if m.distance < 0.7 * n.distance:
        good.append(m)

if len(good) > MIN_MATCH_COUNT:
    # 在原始图像和训练图像中查询特征点
    src_pts = np.float32([kp1[m.queryIdx].pt for m in good]).reshape(-1, 1, 2)
    dst_pts = np.float32([kp2[m.trainIdx].pt for m in good]).reshape(-1, 1, 2)

    # 单应性
    M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
    matchesMask = mask.ravel().tolist()

    # 对第二张图片计算相对于原始图像的投影畸变,并绘制边框
    h, w = img1.shape
    pts = np.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)
    dst = cv2.perspectiveTransform(pts, M)
    img2 = cv2.polylines(img2, [np.int32(dst)], True, 255, 3, cv2.LINE_AA)
else:
    print("Not enough matches are found -%d/%d" % (len(good), MIN_MATCH_COUNT))
    matchesMask = None

# 显示图像
draw_params = dict(matchColor=(0, 255, 0),  # 绿线
                   singlePointColor=None,
                   matchesMask=matchesMask,
                   flags=2)
img3 = cv2.drawMatches(img1, kp1, img2, kp2, good, None, **draw_params)
plt.imshow(img3, 'gray'), plt.show()
