using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestApp
{
    internal class MethodTest
    {
        public int sum(int a, int b)
        {
            return a + b;
        }

        public int recursive(int count)
        {
            if (count == 0) return 0;
            return recursive(count - 1) + count;
        }
    }
}
