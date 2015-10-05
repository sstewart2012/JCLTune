package ca.uwaterloo.jcltune;

import java.util.List;
import java.util.function.Function;

/**
 * Wrapper class for CLTuner C++ Tuner object. Calls native functions. The dispose method should be
 * called explicitly by the user when finished.
 * 
 * Please visit https://github.com/CNugteren/CLTune for more detailed documentation.
 */
public final class JTuner {

  /*
   * Loads the jcltune native library.
   * 
   * "libjcltune.so" for Linux. "libjcltune.dylib" for Apple.
   * 
   * An UnsatisfiedLinkError will be thrown if the JVM cannot find the native library. Be sure to
   * define java.library.path correctly.
   * 
   * Note: on Ubuntu, I also had to set the LD_LIBRARY_PATH environment variable in the run configuration,
   * and I had to run ldconfig to configure dynamic linker run-time bindings.
   * 
   * For guidance: http://examples.javacodegeeks.com/java-basics/java-library-path-what-is-it-and-how-to-use/
   */
  static {
    System.loadLibrary("jcltune");
  }

  /** A handle (memory address) to the C++ Tuner object. */
  private long nativeHandle;

  /**
   * Creates a JTuner for the specified device on the specified platform.
   * 
   * @param platformId The ID for the OpenCL platform.
   * @param deviceId The ID for the OpenCL device on the platform.
   */
  public JTuner(final int platformId, final int deviceId) {
    init(platformId, deviceId);
  }

  @Override
  public String toString() {
    return "JTuner [nativeHandle=" + nativeHandle + "]";
  }

  /**
   * Initialization; constructs the underlying C++ object.
   * 
   * @param platformId The ID for the OpenCL platform.
   * @param deviceId The ID for the OpenCL device on the platform.
   */
  private native void init(int platformId, int deviceId);

  /**
   * Deletes the underlying C++ object. This method should be explicitly called when finished with
   * the tuner.
   */
  public native void dispose();

  public native int addKernel(List<String> filenames, String kernelName, int[] global, int[] local);

  public native int addKernelFromString(String source, String kernelName, int[] global, int[] local);

  public native void setReference(List<String> filenames, String kernelName, int[] global,
      int[] local);

  public native void setReferenceFromString(String source, String kernelName, long[] global,
      long[] local);

  public native void addParameter(int id, String parameterName, int[] values);

  public native void addParameterReference(String parameterName, int value);

  public native void mulGlobalSize(int id, String[] range);

  public native void divGlobalSize(int id, String[] range);

  public native void mulLocalSize(int id, String[] range);

  public native void divLocalSize(int id, String[] range);

  public native void addConstraint(int id, Function<List<Integer>, Boolean> validIf,
      List<String> parameters);

  public native void setLocalMemoryUsage(int id, Function<List<Integer>, Integer> amount,
      List<String> parameters);

  public native <T> void addArgumentInput(List<T> source);

  public native <T> void addArgumentOutput(List<T> source);

  public native <T> void addArgumentScalar(T argument);

  public native void useFullSearch();

  public native void useRandomSearch(double fraction);

  public native void useAnnealing(double fraction, double maxTemperature);

  public native void usePSO(double fraction, int swarmSize, double influenceGlobal,
      double influenceLocal, double influenceRandom);

  public native void outputSearchLog(String filename);

  public native void tune();

  public native double printToScreen();

  public native void printFormatted();

  public native void printJSON(String filename, List<Pair<String, String>> descriptions);

  public native void printToFile(String filename);

  public native void suppressOutput();

}


/**
 * Generic pair class.
 * 
 * @param <L> Left entry of the pair.
 * @param <R> Right entry of the pair.
 */
class Pair<L, R> {
  L first;
  R second;

  public Pair(L first, R second) {
    this.first = first;
    this.second = second;
  }
}
