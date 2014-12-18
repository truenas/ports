--- ./eventlet/support/greendns.py	2013-08-20 05:50:45.000000000 -0700
+++ ./eventlet/support/greendns.py	2014-12-17 19:08:51.897281866 -0800
@@ -34,6 +34,7 @@
 # THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 # OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
+import struct
 
 from eventlet import patcher
 from eventlet.green import _socket_nodns
