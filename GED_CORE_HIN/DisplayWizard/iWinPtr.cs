using Avalonia;
using Avalonia.Input;
using GED.Core.Ctrl;
using GED.Core.SanityCheck;

using float_t = @ae2f_float@;

namespace GED.Core.DisplayWizard {
    /// <summary>
    /// Window from Avalonia, Minimum Control included.
    /// </summary>
    public interface iWinPtr : iWin
    {
        byte iWin.Main(object _prm) {
            win.PointerMoved += OnPointerMoved;
            return States.OK;
        }

        private unsafe void OnPointerMoved(object? sender, PointerEventArgs e)
        {
            Point pos = e.GetPosition(this.win);
            fMousePoint.X[0] = (float_t)pos.X;
            fMousePoint.Y[0] = (float_t)pos.Y;
        }

        public class Built<T> : iWin.Built<T, object> where T : iWinBuff, new()
        {
            public Built(out byte err, object prm) : base(out err, prm) {}
        }
    }
}