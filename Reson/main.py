import cv2  
import torch  
from models.experimental import attempt_load  
from utils.general import non_max_suppression, scale_boxes  
from utils.augmentations import letterbox  
import numpy as np  
  
def process_frame(frame, model, device):  
    # 预处理：letterbox，转换为Tensor，归一化  
    img = letterbox(frame, new_shape=640)[0]  
    img = img[:, :, ::-1].transpose(2, 0, 1)  # BGR to RGB, to 3x416x416  
    img = np.ascontiguousarray(img)  
    img_tensor = torch.from_numpy(img).to(device).float()  
    img_tensor = img_tensor.unsqueeze(0)  
  
    # 推理  
    pred = model(img_tensor)[0]  
    pred = non_max_suppression(pred, 0.9999, 0.001, classes=None, agnostic=False)  
  
    # 处理结果  
    for det in pred:  
        if len(det):  
            det[:, :4] = scale_boxes(img.shape[1:], det[:, :4], frame.shape).round()  
            for *xyxy, conf, cls in det:  
                label = f'{cls} {conf:.2f}'  
                x1, y1, x2, y2 = map(int, xyxy)  
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)  
                cv2.putText(frame, label, (x1, y1 - 7), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)  
    return frame  
  
def main(video_source=0):  
    # 检查 CUDA 是否可用，并设置 device  
    device = torch.device('cuda:0' if torch.cuda.is_available() else 'cpu')  
  
    # 加载模型到指定的 device  
    model = attempt_load('yolov5s.pt').to(device)  
    model.eval()  
  
    # 图像读取和预处理  
    cap = cv2.VideoCapture(video_source)  
    if not cap.isOpened():  
        print("Error: Could not open video stream or file")  
        return  
  
    try:  
        while True:  
            ret, frame = cap.read()  
            if not ret:  
                break  
  
            frame = process_frame(frame, model, device)  
  
            # 显示结果  
            cv2.imshow('YOLOv5 Detection', frame)  
  
            if cv2.waitKey(1) & 0xFF == ord('q'):  
                break  
    finally:  
        cap.release()  
        cv2.destroyAllWindows()  
  
if __name__ == "__main__":  
    main()