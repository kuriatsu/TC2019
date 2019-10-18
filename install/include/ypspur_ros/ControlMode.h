// Generated by gencpp from file ypspur_ros/ControlMode.msg
// DO NOT EDIT!


#ifndef YPSPUR_ROS_MESSAGE_CONTROLMODE_H
#define YPSPUR_ROS_MESSAGE_CONTROLMODE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace ypspur_ros
{
template <class ContainerAllocator>
struct ControlMode_
{
  typedef ControlMode_<ContainerAllocator> Type;

  ControlMode_()
    : vehicle_control_mode(0)  {
    }
  ControlMode_(const ContainerAllocator& _alloc)
    : vehicle_control_mode(0)  {
  (void)_alloc;
    }



   typedef uint8_t _vehicle_control_mode_type;
  _vehicle_control_mode_type vehicle_control_mode;



  enum {
    OPEN = 0u,
    TORQUE = 1u,
    VELOCITY = 2u,
  };


  typedef boost::shared_ptr< ::ypspur_ros::ControlMode_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::ypspur_ros::ControlMode_<ContainerAllocator> const> ConstPtr;

}; // struct ControlMode_

typedef ::ypspur_ros::ControlMode_<std::allocator<void> > ControlMode;

typedef boost::shared_ptr< ::ypspur_ros::ControlMode > ControlModePtr;
typedef boost::shared_ptr< ::ypspur_ros::ControlMode const> ControlModeConstPtr;

// constants requiring out of line definition

   

   

   



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::ypspur_ros::ControlMode_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::ypspur_ros::ControlMode_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace ypspur_ros

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'ypspur_ros': ['/home/kuriatsu/Program/Ros/mad_mobile_ws/src/ypspur_ros/msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::ypspur_ros::ControlMode_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::ypspur_ros::ControlMode_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::ypspur_ros::ControlMode_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::ypspur_ros::ControlMode_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::ypspur_ros::ControlMode_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::ypspur_ros::ControlMode_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::ypspur_ros::ControlMode_<ContainerAllocator> >
{
  static const char* value()
  {
    return "93c0f4fc5070bdb87f70b0914a2708f4";
  }

  static const char* value(const ::ypspur_ros::ControlMode_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x93c0f4fc5070bdb8ULL;
  static const uint64_t static_value2 = 0x7f70b0914a2708f4ULL;
};

template<class ContainerAllocator>
struct DataType< ::ypspur_ros::ControlMode_<ContainerAllocator> >
{
  static const char* value()
  {
    return "ypspur_ros/ControlMode";
  }

  static const char* value(const ::ypspur_ros::ControlMode_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::ypspur_ros::ControlMode_<ContainerAllocator> >
{
  static const char* value()
  {
    return "uint8 vehicle_control_mode\n\
\n\
uint8 OPEN = 0\n\
uint8 TORQUE = 1\n\
uint8 VELOCITY = 2\n\
\n\
";
  }

  static const char* value(const ::ypspur_ros::ControlMode_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::ypspur_ros::ControlMode_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.vehicle_control_mode);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct ControlMode_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::ypspur_ros::ControlMode_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::ypspur_ros::ControlMode_<ContainerAllocator>& v)
  {
    s << indent << "vehicle_control_mode: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.vehicle_control_mode);
  }
};

} // namespace message_operations
} // namespace ros

#endif // YPSPUR_ROS_MESSAGE_CONTROLMODE_H
