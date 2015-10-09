package ca.uwaterloo.jcltune.examples;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

import ca.uwaterloo.jcltune.JTuner;
import ca.uwaterloo.jcltune.Pair;

public class Simple {

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

    final int M = 2048;
    final int N = 4096;

    // Create data structures
    float[] mat_A = new float[N * M]; // Assumes matrix A is transposed
    float[] vec_x = new float[N];
    float[] vec_y = new float[M];

    // Create a random number generator
    Random rand = new Random();

    // Populate input data structures
    for (int i = 0; i < M * N; i++)
      mat_A[i] = rand.nextFloat();
    for (int i = 0; i < N; i++)
      vec_x[i] = rand.nextFloat();
    for (int i = 0; i < M; i++)
      vec_y[i] = rand.nextFloat();

    // Initializes the tuner (platform 0, device 0)
    JTuner tuner = new JTuner(platformId, deviceId);

    // Adds a kernel which supports unrolling through the UNROLL parameter. Note that the kernel
    // itself needs to implement the UNROLL parameter and (in this case) only accepts a limited
    // amount of values.
    int id =
        tuner.addKernel(
            new ArrayList<String>(Arrays.asList("samples/simple/simple_unroll.opencl")),
            "matvec_unroll", new int[] {M}, new int[] {128});
    tuner.addParameter(id, "UNROLL", 1, 2, 4);

    // Adds another kernel and its parameters. This kernel caches the input vector X into local
    // memory to save global memory accesses. Note that the kernel's workgroup size is determined by
    // the tile size parameter TS.
    id =
        tuner.addKernel(new ArrayList<String>(Arrays.asList("samples/simple/simple_tiled.opencl")),
            "matvec_tiled", new int[] {M}, new int[] {1});
    tuner.addParameter(id, "TS", 32, 64, 128, 256, 512);
    tuner.mulLocalSize(id, "TS");

    // Sets the tuner's golden reference function. This kernel contains the reference code to which
    // the output is compared. Supplying such a function is not required, but it is necessarily for
    // correctness checks to be enabled.
    tuner.setReference(
        new ArrayList<String>(Arrays.asList("samples/simple/simple_reference.opencl")),
        "matvec_reference", new int[] {M}, new int[] {128});

    // Sets the function's arguments. Note that all kernels have to accept (but not necessarily use)
    // all input arguments.
    //tuner.addArgumentScalar(kSizeM);
    //tuner.addArgumentScalar(kSizeN);
    //tuner.addArgumentInput(mat_a);
    //tuner.addArgumentInput(vec_x);
    //tuner.addArgumentOutput(vec_y);

    // Starts the tuner
    tuner.tune();

    // Prints the results to screen
    tuner.printToScreen();

    // Outputs all tuning results to JSON for later analysis. The argument provided here is an extra
    // JSON entry: the name of this 'sample' is set to 'simple'.
    ArrayList<Pair<String,String>> descriptions = new ArrayList<>();
    descriptions.add(new Pair<String, String>("simple", "simple"));
    tuner.printJSON("output.json", descriptions);

    tuner.dispose();
  }
}
