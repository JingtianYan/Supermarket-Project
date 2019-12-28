
import cv2
import numpy as np
from os import walk
from os.path import join
import sys


# 创建描述符
def create_descriptors(folder):
    files = []
    for (dirpath, dirnames, filenames) in walk(folder):
        files.extend(filenames)
    for f in files:
        save_descriptor(folder, f, cv2.xfeatures2d.SIFT_create())


# 保存描述符
def save_descriptor(folder, image_path, feature_detector):
    print("reading %s" % image_path)
    if image_path.endswith("npy") or image_path.endswith("avi"):
        return
    img = cv2.imread(join(folder, image_path), cv2.IMREAD_GRAYSCALE)
    keypoints, descriptors = feature_detector.detectAndCompute(img, None)
    descriptor_file = image_path.replace("jpg", "npy")
    np.save(join(folder, descriptor_file), descriptors)


# 从执行参数中取得文件目录
dir = 'test'
create_descriptors(dir)
