using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using app2.Data;
using app2.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Options;

namespace app2.Controllers
{
    [Route("api/[controller]")]
    public class AlarmesController : Controller
    {

        public AlarmesController(app2Context dbContext)
        {
            DbContext = dbContext;
        }

        public app2Context DbContext { get; }

        // GET api/values
        [HttpGet]
        public async Task<IActionResult> Get()
        {
            return Ok(await DbContext.Alarmes.ToListAsync());
        }

        // GET api/values/5
        [HttpGet("{id}")]
        public async Task<IActionResult> Get(Guid id) //read
        {
            return Ok(await DbContext.Alarmes.SingleOrDefaultAsync(m => m.Id == id));

        }

         [HttpPost()]
        public async Task<IActionResult> Post([FromBody]Alarme value)
        {
            if (value != null)
            {
                await DbContext.Alarmes.AddAsync(value);
                await DbContext.SaveChangesAsync();
                return Ok(value);
            }
            else
            {
                return BadRequest();
            }

        }


        [HttpPut("{id}")]
        public async Task<IActionResult> Put(Guid id, [FromBody] Alarme value)
        {
            if (value == null || value.Id != id)
            {
                return BadRequest();
            }

            var updateValue = await DbContext.Alarmes.FirstOrDefaultAsync(t => t.Id == id);
            
            if (updateValue == null)
            {
                return NotFound();
            }

            updateValue.hora = value.hora; //update name
            updateValue.minuto = value.minuto;
            DbContext.Alarmes.Update(updateValue);
            await DbContext.SaveChangesAsync();
            return new NoContentResult();
        }

        // DELETE api/values/5
        [HttpDelete("{id}")]
        public async Task<IActionResult> Delete(Guid id)
        {
            var Alarmes = await DbContext.Alarmes.SingleOrDefaultAsync(m => m.Id == id);
            DbContext.Alarmes.Remove(Alarmes);
            await DbContext.SaveChangesAsync();
            return NoContent();
        }
    }
}
