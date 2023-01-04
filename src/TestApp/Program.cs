using System;

namespace TestApp
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("enter to test #1 - sum");
            Console.ReadLine();
            Console.WriteLine(sum(1, 2).ToString());

            Console.WriteLine("enter to test #2 - recursive");
            Console.ReadLine();
            MethodTest methodTest = new MethodTest();
            Console.WriteLine(methodTest.recursive(3).ToString());

            Console.WriteLine("enter to close this program");
            Console.ReadLine();
        }

        static int sum(int a, int b)
        {
            MethodTest methodTest = new MethodTest();
            return methodTest.sum(a, b);
        }
    }
}
