
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ColorGroupsWrapper
{
    [StructLayout(LayoutKind.Sequential)]
    public class ColorGrps
    {
        

       [DllImport("ColorGroupsDyn.dll", CharSet = CharSet.Unicode)]
        public static extern IntPtr CreateColorGroups(string pathName);

        [DllImport("ColorGroupsDyn.dll", CharSet = CharSet.Unicode)]
        public static extern IntPtr CreateColorGroupsWithBitmap( IntPtr bitmap,int w,int h);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern int GetGroupCount(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern int GetBlackCount(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern int GetBlueCount(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern int GetRedCount(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern int GetGreenCount(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern int GetYellowCount(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern int GetOrangeCount(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern int GetPinkCount(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern IntPtr GetUnknownCount(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern IntPtr ShowBlackColorGroups(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern IntPtr ShowBlueColorGroups(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern IntPtr ShowRedColorGroups(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern IntPtr ShowGreenColorGroups(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern IntPtr ShowOrangeColorGroups(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern IntPtr ShowYellowColorGroups(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern IntPtr ShowPinkColorGroups(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern IntPtr ShowUnknownColorGroups(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern int GetWidth(IntPtr obj);

        [DllImport("ColorGroupsDyn.dll")]
        public static extern int GetHeight(IntPtr obj);

    }
}
