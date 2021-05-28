using System;
using System.Collections.Generic;
using System.Text;

namespace BI_Inlupp_1.Models
{
    class Device
    {
        public string DeviceId { get; set; }
        public int TimeSent { get; set; }
        public float Temperature { get; set; }
        public float Humidity { get; set; }
    }
}
