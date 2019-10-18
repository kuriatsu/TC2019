;; Auto-generated. Do not edit!


(when (boundp 'test_tools::IMUPacket)
  (if (not (find-package "TEST_TOOLS"))
    (make-package "TEST_TOOLS"))
  (shadow 'IMUPacket (find-package "TEST_TOOLS")))
(unless (find-package "TEST_TOOLS::IMUPACKET")
  (make-package "TEST_TOOLS::IMUPACKET"))

(in-package "ROS")
;;//! \htmlinclude IMUPacket.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass test_tools::IMUPacket
  :super ros::object
  :slots (_header _wx _wy _wz _ax _ay _az _wheel ))

(defmethod test_tools::IMUPacket
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:wx __wx) 0)
    ((:wy __wy) 0)
    ((:wz __wz) 0)
    ((:ax __ax) 0)
    ((:ay __ay) 0)
    ((:az __az) 0)
    ((:wheel __wheel) 0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _wx (round __wx))
   (setq _wy (round __wy))
   (setq _wz (round __wz))
   (setq _ax (round __ax))
   (setq _ay (round __ay))
   (setq _az (round __az))
   (setq _wheel (round __wheel))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:wx
   (&optional __wx)
   (if __wx (setq _wx __wx)) _wx)
  (:wy
   (&optional __wy)
   (if __wy (setq _wy __wy)) _wy)
  (:wz
   (&optional __wz)
   (if __wz (setq _wz __wz)) _wz)
  (:ax
   (&optional __ax)
   (if __ax (setq _ax __ax)) _ax)
  (:ay
   (&optional __ay)
   (if __ay (setq _ay __ay)) _ay)
  (:az
   (&optional __az)
   (if __az (setq _az __az)) _az)
  (:wheel
   (&optional __wheel)
   (if __wheel (setq _wheel __wheel)) _wheel)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; int16 _wx
    2
    ;; int16 _wy
    2
    ;; int16 _wz
    2
    ;; int16 _ax
    2
    ;; int16 _ay
    2
    ;; int16 _az
    2
    ;; int16 _wheel
    2
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; int16 _wx
       (write-word _wx s)
     ;; int16 _wy
       (write-word _wy s)
     ;; int16 _wz
       (write-word _wz s)
     ;; int16 _ax
       (write-word _ax s)
     ;; int16 _ay
       (write-word _ay s)
     ;; int16 _az
       (write-word _az s)
     ;; int16 _wheel
       (write-word _wheel s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; int16 _wx
     (setq _wx (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _wy
     (setq _wy (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _wz
     (setq _wz (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _ax
     (setq _ax (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _ay
     (setq _ay (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _az
     (setq _az (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _wheel
     (setq _wheel (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;;
   self)
  )

(setf (get test_tools::IMUPacket :md5sum-) "5d4a54def277a6afd729e1ac2194dc07")
(setf (get test_tools::IMUPacket :datatype-) "test_tools/IMUPacket")
(setf (get test_tools::IMUPacket :definition-)
      "Header header
int16 wx
int16 wy
int16 wz
int16 ax
int16 ay
int16 az
int16 wheel

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

")



(provide :test_tools/IMUPacket "5d4a54def277a6afd729e1ac2194dc07")


