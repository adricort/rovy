#ifndef _ROS_aruco_analyzer_sys_resources_h
#define _ROS_aruco_analyzer_sys_resources_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace aruco_analyzer
{

  class sys_resources : public ros::Msg
  {
    public:
      typedef float _cpu_type;
      _cpu_type cpu;
      typedef float _mem_type;
      _mem_type mem;

    sys_resources():
      cpu(0),
      mem(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += serializeAvrFloat64(outbuffer + offset, this->cpu);
      offset += serializeAvrFloat64(outbuffer + offset, this->mem);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->cpu));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->mem));
     return offset;
    }

    virtual const char * getType() override { return "aruco_analyzer/sys_resources"; };
    virtual const char * getMD5() override { return "22ce339c1cf5819c99000902446966ad"; };

  };

}
#endif
