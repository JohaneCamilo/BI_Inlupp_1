using System;
using System.IO;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Extensions.Http;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using System.Data.SqlClient;
using System.Data;
using System.Collections.Generic;

namespace Buisness_Inteligence_v2
{
    public static class GetFromSql
    {
        [FunctionName("GetFromSql")]
        public static async Task<IActionResult> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "get", Route = null)] HttpRequest req,
            ILogger log)
        {
            var results = new List<Dictionary<string, object>>();
            string new_message;

            log.LogInformation("C# HTTP trigger function processed a request.");
            using (var connection = new SqlConnection(Environment.GetEnvironmentVariable("SqlConnection")))
            {
                var query = "SELECT * FROM DeviceMessages";
                connection.Open();
                using (var cmd = new SqlCommand(query, connection))
                {
                    var reader = cmd.ExecuteReader();
                    while (reader.Read())
                    {
                        var dict = new Dictionary<string, object>
                        {
                            ["deviceid"] = reader[1].ToString(),
                            ["timesent"] = reader[2].ToString(),
                            ["temperature"] = reader[3].ToString(),
                            ["humidity"] = reader[4].ToString(),
                        };
                        results.Add(dict);
                    }
                    new_message = JsonConvert.SerializeObject(results);
                }
            }
            return new OkObjectResult(new_message);
        }
    }
}
