// Auto-generated. Do not edit!

// (in-package test_tools.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class IMUPacket {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.wx = null;
      this.wy = null;
      this.wz = null;
      this.ax = null;
      this.ay = null;
      this.az = null;
      this.wheel = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('wx')) {
        this.wx = initObj.wx
      }
      else {
        this.wx = 0;
      }
      if (initObj.hasOwnProperty('wy')) {
        this.wy = initObj.wy
      }
      else {
        this.wy = 0;
      }
      if (initObj.hasOwnProperty('wz')) {
        this.wz = initObj.wz
      }
      else {
        this.wz = 0;
      }
      if (initObj.hasOwnProperty('ax')) {
        this.ax = initObj.ax
      }
      else {
        this.ax = 0;
      }
      if (initObj.hasOwnProperty('ay')) {
        this.ay = initObj.ay
      }
      else {
        this.ay = 0;
      }
      if (initObj.hasOwnProperty('az')) {
        this.az = initObj.az
      }
      else {
        this.az = 0;
      }
      if (initObj.hasOwnProperty('wheel')) {
        this.wheel = initObj.wheel
      }
      else {
        this.wheel = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type IMUPacket
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [wx]
    bufferOffset = _serializer.int16(obj.wx, buffer, bufferOffset);
    // Serialize message field [wy]
    bufferOffset = _serializer.int16(obj.wy, buffer, bufferOffset);
    // Serialize message field [wz]
    bufferOffset = _serializer.int16(obj.wz, buffer, bufferOffset);
    // Serialize message field [ax]
    bufferOffset = _serializer.int16(obj.ax, buffer, bufferOffset);
    // Serialize message field [ay]
    bufferOffset = _serializer.int16(obj.ay, buffer, bufferOffset);
    // Serialize message field [az]
    bufferOffset = _serializer.int16(obj.az, buffer, bufferOffset);
    // Serialize message field [wheel]
    bufferOffset = _serializer.int16(obj.wheel, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type IMUPacket
    let len;
    let data = new IMUPacket(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [wx]
    data.wx = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [wy]
    data.wy = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [wz]
    data.wz = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [ax]
    data.ax = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [ay]
    data.ay = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [az]
    data.az = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [wheel]
    data.wheel = _deserializer.int16(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    return length + 14;
  }

  static datatype() {
    // Returns string type for a message object
    return 'test_tools/IMUPacket';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '5d4a54def277a6afd729e1ac2194dc07';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header header
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new IMUPacket(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.wx !== undefined) {
      resolved.wx = msg.wx;
    }
    else {
      resolved.wx = 0
    }

    if (msg.wy !== undefined) {
      resolved.wy = msg.wy;
    }
    else {
      resolved.wy = 0
    }

    if (msg.wz !== undefined) {
      resolved.wz = msg.wz;
    }
    else {
      resolved.wz = 0
    }

    if (msg.ax !== undefined) {
      resolved.ax = msg.ax;
    }
    else {
      resolved.ax = 0
    }

    if (msg.ay !== undefined) {
      resolved.ay = msg.ay;
    }
    else {
      resolved.ay = 0
    }

    if (msg.az !== undefined) {
      resolved.az = msg.az;
    }
    else {
      resolved.az = 0
    }

    if (msg.wheel !== undefined) {
      resolved.wheel = msg.wheel;
    }
    else {
      resolved.wheel = 0
    }

    return resolved;
    }
};

module.exports = IMUPacket;
