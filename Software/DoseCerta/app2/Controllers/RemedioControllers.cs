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
    public class RemedioController : Controller
    {

        public RemedioController(app2Context dbContext)
        {
            DbContext = dbContext;
        }

        public app2Context DbContext { get; }

        // GET api/values
        [HttpGet]
        public async Task<IActionResult> Get()
        {
            return Ok(await DbContext.Remedio.ToListAsync());
        }

        // GET api/values/5
        [HttpGet("{id}")]
        public async Task<IActionResult> Get(Guid id) //read
        {
            return Ok(await DbContext.Remedio.SingleOrDefaultAsync(m => m.Id == id));

        }

         [HttpPost()]
        public async Task<IActionResult> Post([FromBody]Remedio value)
        {
            if (value != null)
            {
                await DbContext.Remedio.AddAsync(value);
                await DbContext.SaveChangesAsync();
                return new NoContentResult();
            }
            else
            {
                return BadRequest();
            }

        }


        [HttpPut("{id}")]
        public async Task<IActionResult> Put(Guid id, [FromBody] Remedio value)
        {
            if (value == null || value.Id != id)
            {
                return BadRequest();
            }

            var updateValue = await DbContext.Remedio.FirstOrDefaultAsync(t => t.Id == id);
            
            if (updateValue == null)
            {
                return NotFound();
            }

            updateValue.Nome = value.Nome; //update name
            updateValue.Quantidade = value.Quantidade;
            updateValue.Intervalo = value.Intervalo;
            DbContext.Remedio.Update(updateValue);
            await DbContext.SaveChangesAsync();
            return new NoContentResult();
        }

        // DELETE api/values/5
        [HttpDelete("{id}")]
        public async Task<IActionResult> Delete(Guid id)
        {
            var Remedio = await DbContext.Remedio.SingleOrDefaultAsync(m => m.Id == id);
            DbContext.Remedio.Remove(Remedio);
            await DbContext.SaveChangesAsync();
            return NoContent();
        }
    }
}
