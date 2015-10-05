package ca.uwaterloo.jcltune;

import java.util.ArrayList;
import java.util.Arrays;

public class Main {

  public static void main(String[] args) {
    final int platformId;
    final int deviceId;

    // Check if platformId and deviceId were passed as arguments
    if (args.length == 2) {
      platformId = Integer.parseInt(args[0]);
      deviceId = Integer.parseInt(args[1]);
    } else {
      platformId = 0;
      deviceId = 0;
    }

    // Constants
    final int N = 1024; // host array size
    final int WGSZ = 512; // workgroup (local) size

    // Host data
    int[] A = new int[N];
    int[] B = new int[N];

    // Set up tuner
    JTuner myTuner = new JTuner(platformId, deviceId);
    final int kernelId =
        myTuner.addKernel(new ArrayList<String>(Arrays.asList("kernels/hello.cl")), "hello", new int[] {N},
            new int[] {WGSZ});

    // Add tuning parameters
    myTuner.addParameter(kernelId, "ABC", new int[] {32, 64});

    // Set reference
    myTuner.setReference(new ArrayList<String>(Arrays.asList("kernels/hello.cl")), "hello", new int[] {N},
        new int[] {WGSZ});

    // Deletes the native C++ object
    myTuner.dispose();
  }
}
