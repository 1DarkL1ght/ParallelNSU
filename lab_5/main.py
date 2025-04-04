import argparse
import threading
import queue
import time
from timeit import default_timer as timer

import ultralytics
from ultralytics import YOLO
import cv2

stop_event = threading.Event()

class Detector:
    def __init__(self):
        self._detection_model = YOLO("yolov8n-pose.pt", verbose=False)

    
    def detect(self, queue: queue.Queue, output:list):
        while not stop_event.is_set():
            if not queue.empty():
                frame, id = queue.get()
                output.append((self._detection_model(frame, verbose=False)[0].plot(), id))
                queue.task_done()



def main():
    output_frames = []
    num_threads = 4

    parser = argparse.ArgumentParser()
    parser.add_argument('-path', type=str, default='input.mp4')
    parser.add_argument('-mode', type=str, default='single')
    parser.add_argument('-output', type=str, default='output.mp4')
    args = parser.parse_args()

    if args.mode == 'single':
        num_threads = 1

    print(f'Starting with {args.path}')

    frame_queue = queue.Queue(maxsize=30)
    detectors = [Detector() for i in range(num_threads)]
    threads = [threading.Thread(target=detectors[i].detect, args=(frame_queue, output_frames)) for i in range(num_threads)]

    video = cv2.VideoCapture(args.path)



    frame_id = 0
    stop_flag = False
    start = timer()
    while video.isOpened():
        for thread in threads:
            if thread.ident is None:
                thread.start()

        while not frame_queue.full():
            ret, frame = video.read()
            print(f'Got frame {frame_id}')
            if not ret:
                print("Unable to get frame")
                stop_flag = True
                break
            frame_queue.put((frame, frame_id))
            frame_id += 1
        if stop_flag:
            break


    frame_queue.join()
    stop_event.set()
    for t in threads:
        t.join()
    # while True:
    #     if frame_queue.empty():
    #         stop_event.set()
    #         for thread in threads:
    #             thread.join()
    #         break

    end = timer()
    print("Done processing frames")
    print(f'Elapsed time on {num_threads} thread(s): {end - start}s')
    output_frames.sort(key=lambda x: x[1])

    framerate = 30
    for frame, _ in output_frames:
        cv2.imshow('Image', frame)
        time.sleep(1 / framerate)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    
    video.release()
    cv2.destroyAllWindows()


main()