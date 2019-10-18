
(cl:in-package :asdf)

(defsystem "test_tools-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "IMUPacket" :depends-on ("_package_IMUPacket"))
    (:file "_package_IMUPacket" :depends-on ("_package"))
    (:file "Vibration" :depends-on ("_package_Vibration"))
    (:file "_package_Vibration" :depends-on ("_package"))
  ))