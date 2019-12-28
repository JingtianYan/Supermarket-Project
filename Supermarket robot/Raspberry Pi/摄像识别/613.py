
import cv2
import sys
import numpy as py

# 读入图像
# imgpath=sys.argv[1]
# alg=sys.argv[2]
# threshold=sys.argv[3]

imgpath = '1.jpg'
img = cv2.imread(imgpath)
alg = 'SURF'
#alg = 'SIFT'
# threshold = '8000'
# 阈值越小特征点越多
threshold = '1000'


def fd(algorithm):
    if algorithm == 'SIFT':
        return cv2.xfeatures2d.SIFT_create()
    if algorithm == 'SURF':
        # return cv2.xfeatures2d.SURF_create(float(threshold) if len(sys.argv) == 4 else 4000)
        return cv2.xfeatures2d.SURF_create(float(threshold))


# 更换颜色空间
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# 创建sift对象,计算灰度图像,会使用dog检测角点
fd_alg = fd(alg)
keypoints, descriptor = fd_alg.detectAndCompute(gray, None)

# print(keypoints)
# 关键点有以下几个属性
# angle 表示特征的方向
# class_id 关键点的id
# octave 特征所在金字塔的等级
# pt 图像中关键点的坐标
# response 表示关键点的强度
# size  表示特征的直径
img = cv2.drawKeypoints(image=img,
                        outImage=img,
                        keypoints=keypoints,
                        color=(51, 163, 236),
                        flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

# 显示图像
cv2.imshow('keypoints', img)
while (True):
    if cv2.waitKey(int(1000 / 12)) & 0xFF == ord('q'):
        break
cv2.destroyAllWindows()
