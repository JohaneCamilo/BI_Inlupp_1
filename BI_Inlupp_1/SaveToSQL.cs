using IoTHubTrigger = Microsoft.Azure.WebJobs.EventHubTriggerAttribute;

using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Host;
using Microsoft.Azure.EventHubs;
using System.Text;
using System.Net.Http;
using Microsoft.Extensions.Logging;
using System;
using System.Data.SqlClient;
using Newtonsoft.Json;
using BI_Inlupp_1.Models;

namespace FunctionApp1
{
    public static class SaveToSql
    {
        private static HttpClient client = new HttpClient();

        [FunctionName("SaveToSql")]
        public static void Run([IoTHubTrigger("messages/events", Connection = "Camilo-hubb")] EventData message, ILogger log)
        {
            log.LogInformation($"Message IN: {Encoding.UTF8.GetString(message.Body.Array)}");
            var Name = message.Properties["Name"].ToString();
            var School = message.Properties["School"].ToString();

            var _message = JsonConvert.DeserializeObject<Device>(Encoding.UTF8.GetString(message.Body.Array));

            using (var conn = new SqlConnection(Environment.GetEnvironmentVariable("SqlConnection")))
            {
                var query = "INSERT INTO DeviceMessages VALUES(@deviceid, @timesent, @temperature, @humidity)";

                conn.Open();
                using (var cmd = new SqlCommand(query, conn))
                {
                    cmd.Parameters.AddWithValue("@deviceid", _message.DeviceId);
                    cmd.Parameters.AddWithValue("@timesent", _message.TimeSent);
                    cmd.Parameters.AddWithValue("@temperature", _message.Temperature);
                    cmd.Parameters.AddWithValue("@humidity", _message.Humidity);

                    cmd.ExecuteNonQuery();
                }
            }
        }
    }
}
