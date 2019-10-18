; Auto-generated. Do not edit!


(cl:in-package test_tools-msg)


;//! \htmlinclude IMUPacket.msg.html

(cl:defclass <IMUPacket> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (wx
    :reader wx
    :initarg :wx
    :type cl:fixnum
    :initform 0)
   (wy
    :reader wy
    :initarg :wy
    :type cl:fixnum
    :initform 0)
   (wz
    :reader wz
    :initarg :wz
    :type cl:fixnum
    :initform 0)
   (ax
    :reader ax
    :initarg :ax
    :type cl:fixnum
    :initform 0)
   (ay
    :reader ay
    :initarg :ay
    :type cl:fixnum
    :initform 0)
   (az
    :reader az
    :initarg :az
    :type cl:fixnum
    :initform 0)
   (wheel
    :reader wheel
    :initarg :wheel
    :type cl:fixnum
    :initform 0))
)

(cl:defclass IMUPacket (<IMUPacket>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <IMUPacket>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'IMUPacket)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name test_tools-msg:<IMUPacket> is deprecated: use test_tools-msg:IMUPacket instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <IMUPacket>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader test_tools-msg:header-val is deprecated.  Use test_tools-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'wx-val :lambda-list '(m))
(cl:defmethod wx-val ((m <IMUPacket>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader test_tools-msg:wx-val is deprecated.  Use test_tools-msg:wx instead.")
  (wx m))

(cl:ensure-generic-function 'wy-val :lambda-list '(m))
(cl:defmethod wy-val ((m <IMUPacket>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader test_tools-msg:wy-val is deprecated.  Use test_tools-msg:wy instead.")
  (wy m))

(cl:ensure-generic-function 'wz-val :lambda-list '(m))
(cl:defmethod wz-val ((m <IMUPacket>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader test_tools-msg:wz-val is deprecated.  Use test_tools-msg:wz instead.")
  (wz m))

(cl:ensure-generic-function 'ax-val :lambda-list '(m))
(cl:defmethod ax-val ((m <IMUPacket>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader test_tools-msg:ax-val is deprecated.  Use test_tools-msg:ax instead.")
  (ax m))

(cl:ensure-generic-function 'ay-val :lambda-list '(m))
(cl:defmethod ay-val ((m <IMUPacket>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader test_tools-msg:ay-val is deprecated.  Use test_tools-msg:ay instead.")
  (ay m))

(cl:ensure-generic-function 'az-val :lambda-list '(m))
(cl:defmethod az-val ((m <IMUPacket>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader test_tools-msg:az-val is deprecated.  Use test_tools-msg:az instead.")
  (az m))

(cl:ensure-generic-function 'wheel-val :lambda-list '(m))
(cl:defmethod wheel-val ((m <IMUPacket>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader test_tools-msg:wheel-val is deprecated.  Use test_tools-msg:wheel instead.")
  (wheel m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <IMUPacket>) ostream)
  "Serializes a message object of type '<IMUPacket>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let* ((signed (cl:slot-value msg 'wx)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'wy)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'wz)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'ax)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'ay)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'az)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'wheel)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <IMUPacket>) istream)
  "Deserializes a message object of type '<IMUPacket>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'wx) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'wy) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'wz) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ax) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ay) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'az) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'wheel) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<IMUPacket>)))
  "Returns string type for a message object of type '<IMUPacket>"
  "test_tools/IMUPacket")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'IMUPacket)))
  "Returns string type for a message object of type 'IMUPacket"
  "test_tools/IMUPacket")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<IMUPacket>)))
  "Returns md5sum for a message object of type '<IMUPacket>"
  "5d4a54def277a6afd729e1ac2194dc07")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'IMUPacket)))
  "Returns md5sum for a message object of type 'IMUPacket"
  "5d4a54def277a6afd729e1ac2194dc07")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<IMUPacket>)))
  "Returns full string definition for message of type '<IMUPacket>"
  (cl:format cl:nil "Header header~%int16 wx~%int16 wy~%int16 wz~%int16 ax~%int16 ay~%int16 az~%int16 wheel~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'IMUPacket)))
  "Returns full string definition for message of type 'IMUPacket"
  (cl:format cl:nil "Header header~%int16 wx~%int16 wy~%int16 wz~%int16 ax~%int16 ay~%int16 az~%int16 wheel~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <IMUPacket>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     2
     2
     2
     2
     2
     2
     2
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <IMUPacket>))
  "Converts a ROS message object to a list"
  (cl:list 'IMUPacket
    (cl:cons ':header (header msg))
    (cl:cons ':wx (wx msg))
    (cl:cons ':wy (wy msg))
    (cl:cons ':wz (wz msg))
    (cl:cons ':ax (ax msg))
    (cl:cons ':ay (ay msg))
    (cl:cons ':az (az msg))
    (cl:cons ':wheel (wheel msg))
))
