/*
 * Filename:  JTuner.cpp
 * Author:    Steven Stewart <steven.stewart@uwaterloo.ca>
 *
 * JTuner JNI wrapper of CLTune public API. Implemented in C/C++.
 *
 * Handy references:
 *  http://thebreakfastpost.com/2012/01/21/wrapping-a-c-library-with-jni-introduction/
 *  http://stackoverflow.com/questions/24403340/jni-java-arraylist-conversion-to-c-stdstring
 */
#include <iostream>
#include <string>

#include "JTuner.h"
#include "handle.h"
#include "cltune.h"
#include "helper.h"

using namespace std;
using namespace cltune;

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_init
(JNIEnv *env, jobject obj, jint platform_id, jint device_id) {
  Tuner *t = new Tuner((size_t)platform_id, (size_t)device_id);
  setHandle(env, obj, t);
}

JNIEXPORT jint JNICALL Java_ca_uwaterloo_jcltune_JTuner_addKernel(
    JNIEnv *env, jobject obj, jobject jfilenames, jstring jkernel_name,
    jintArray jglobal, jintArray jlocal) {
  // Convert java.util.List to vector of string
  vector < string > filenames = listToStringVector(env, jfilenames);

  // Convert arrays to IntRange type
  IntRange global = toIntRange(env, jglobal);
  IntRange local = toIntRange(env, jlocal);

  // Convert jkernel_name to C++ string
  const char *nativeString = env->GetStringUTFChars(jkernel_name, 0);

  // Call function
  Tuner *t = getHandle<Tuner>(env, obj);
  size_t kernelId = t->AddKernel(filenames, string(nativeString), global,
                                 local);

  // Release string
  env->ReleaseStringUTFChars(jkernel_name, nativeString);

  return (jint) kernelId;
}

JNIEXPORT jint JNICALL Java_ca_uwaterloo_jcltune_JTuner_addKernelFromString(
    JNIEnv *env, jobject obj, jstring jsource, jstring jkernel_name,
    jintArray jglobal, jintArray jlocal) {
  // Convert jstring to C++ string
  const char *c_source = env->GetStringUTFChars(jsource, 0);
  const char *c_kernel_name = env->GetStringUTFChars(jkernel_name, 0);
  const string source(c_source);
  const string kernel_name(c_kernel_name);
  env->ReleaseStringUTFChars(jkernel_name, c_source);
  env->ReleaseStringUTFChars(jkernel_name, c_kernel_name);

  // Convert arrays to IntRange type
  IntRange global = toIntRange(env, jglobal);
  IntRange local = toIntRange(env, jlocal);

  // Call function
  Tuner *t = getHandle<Tuner>(env, obj);
  size_t kernelId = t->AddKernelFromString(source, kernel_name, global, local);

  return (jint) kernelId;
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_setReference
(JNIEnv *env, jobject obj, jobject jfilenames, jstring jkernel_name, jintArray jglobal, jintArray jlocal) {
  // Convert jstring to C++ string
  const char *c_kernel_name = env->GetStringUTFChars(jkernel_name, 0);
  const string kernel_name(c_kernel_name);
  env->ReleaseStringUTFChars(jkernel_name, c_kernel_name);

  // Convert java.util.List to vector of string
  vector<string> filenames = listToStringVector(env, jfilenames);

  // Convert arrays to IntRange type
  IntRange global = toIntRange(env, jglobal);
  IntRange local = toIntRange(env, jlocal);

  // Call function
  Tuner *t = getHandle<Tuner>(env, obj);
  t->SetReference(filenames, kernel_name, global, local);
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_setReferenceFromString
(JNIEnv *env, jobject obj, jstring jsource, jstring jkernel_name, jintArray jglobal, jintArray jlocal) {
  // Convert jstring to C++ string
  const char *c_source = env->GetStringUTFChars(jsource, 0);
  const string source(c_source);
  env->ReleaseStringUTFChars(jkernel_name, c_source);
  const char *c_kernel_name = env->GetStringUTFChars(jkernel_name, 0);
  const string kernel_name(c_kernel_name);
  env->ReleaseStringUTFChars(jkernel_name, c_kernel_name);

  // Convert arrays to IntRange type
  IntRange global = toIntRange(env, jglobal);
  IntRange local = toIntRange(env, jlocal);

  // Call function
  Tuner *t = getHandle<Tuner>(env, obj);
  t->SetReferenceFromString(source, kernel_name, global, local);
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_addParameter
(JNIEnv *env, jobject obj, jint jid, jstring jparameter_name, jintArray jvalues) {
  // kernel id
  int id = (int) jid;

  // parameter name
  const string parameter_name = jstringToString(env, jparameter_name);

  // convert jintArray to int[]
  jsize length = env->GetArrayLength(jvalues);
  jint *a = (jint*)env->GetPrimitiveArrayCritical(jvalues, NULL);
  size_t *result = new size_t[length];
  env->ReleasePrimitiveArrayCritical(jvalues, a, JNI_ABORT);
  for (int i = 0; i < length; i++) {
    result[i] = (size_t) a[i];
  }
  env->ReleasePrimitiveArrayCritical(jvalues, a, JNI_ABORT);

  // Call function
  Tuner *t = getHandle<Tuner>(env, obj);
  t->AddParameter(id, parameter_name, {result[0]});
  // TODO solve initializer_list problem
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_addParameterReference
(JNIEnv *env, jobject obj, jstring jparameter_name, jint jvalue) {
  const string parameter_name = jstringToString(env, jparameter_name);
  const size_t value = (size_t) jvalue;
  Tuner *t = getHandle<Tuner>(env, obj);
  t->AddParameterReference(parameter_name, value);
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_mulGlobalSize
(JNIEnv *env, jobject obj, jint jid, jobjectArray jvalues) {
  const size_t id = (int) jid;
  const StringRange values = toStringRange(env, jvalues);
  Tuner *t = getHandle<Tuner>(env, obj);
  t->MulGlobalSize(id, values);
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_divGlobalSize
(JNIEnv *env, jobject obj, jint jid, jobjectArray jvalues) {
  const size_t id = (int) jid;
  const StringRange values = toStringRange(env, jvalues);
  Tuner *t = getHandle<Tuner>(env, obj);
  t->DivGlobalSize(id, values);
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_mulLocalSize
(JNIEnv *env, jobject obj, jint jid, jobjectArray jvalues) {
  const size_t id = (int) jid;
  const StringRange values = toStringRange(env, jvalues);
  Tuner *t = getHandle<Tuner>(env, obj);
  t->MulLocalSize(id, values);
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_divLocalSize
(JNIEnv *env, jobject obj, jint jid, jobjectArray jvalues) {
  const size_t id = (int) jid;
  const StringRange values = toStringRange(env, jvalues);
  Tuner *t = getHandle<Tuner>(env, obj);
  t->DivLocalSize(id, values);
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_addConstraint
(JNIEnv *env, jobject obj, jint jid, jobject jvalid_if, jobject jparameters) {
  const size_t id = (int) jid;
  const StringVec parameters = listToStringVector(env, jparameters);

  // java.util.Function.Predicate
  jclass cPredicate = env->FindClass("java/util/function/Predicate");
  jmethodID mPredicateTest = env->GetMethodID(cPredicate, "test", "(Ljava/lang/Object;)Z");

  // java.util.ArrayList
  jclass cArrayList = env->FindClass("java/util/ArrayList");
  jmethodID mArrayListInit = env->GetMethodID(cArrayList, "<init>",  "()V");
  jmethodID mArrayListAdd = env->GetMethodID(cArrayList, "add", "(Ljava/lang/Object;)Z");

  // java.lang.Integer
  jclass cInteger = env->FindClass("java/lang/Integer");
  jmethodID mIntegerInit = env->GetMethodID(cInteger, "<init>",  "(I)V");

  // Declaration of a function object "valid_if" that calls the Java function "jvalid_if".
  auto valid_if = [env, obj, cPredicate, mPredicateTest, cArrayList, mArrayListInit, 
           mArrayListAdd, cInteger, mIntegerInit, jvalid_if, jparameters] (SizetVec values) -> bool
  {
    // Add each value in the vector to an ArrayList<Integer>
    jobject jArrayList = env->NewObject(cArrayList, mArrayListInit);
    for (int i = 0; i < values.size(); ++i) {
      jobject tmp = env->NewObject(cInteger, mIntegerInit, (jint) values[i]);
      env->CallBooleanMethod(jArrayList, mArrayListAdd, tmp);
    }

    // Calls the test method of the Predicate function object
    jboolean result = (jboolean) env->CallBooleanMethod(jvalid_if, mPredicateTest, jArrayList);
    return (bool) result;
  };

  // Call the AddConstraint method with the function object and the vector of parameters
  Tuner *t = getHandle<Tuner>(env, obj);
  t->AddConstraint(id, valid_if, parameters);
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_setLocalMemoryUsage
(JNIEnv *env, jobject obj, jint jid, jobject jamount, jobject jparameters) {
  const size_t id = (int) jid;
  // TODO amount function
  const StringVec parameters = listToStringVector(env, jparameters);
  Tuner *t = getHandle<Tuner>(env, obj);
  t->AddConstraint(id, nullptr, parameters);
}

// TODO AddArgument functions

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_useFullSearch
(JNIEnv *env, jobject obj) {
  Tuner *t = getHandle<Tuner>(env, obj);
  t->UseFullSearch();
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_useRandomSearch
(JNIEnv *env, jobject obj, jdouble jfraction) {
  Tuner *t = getHandle<Tuner>(env, obj);
  t->UseRandomSearch((double)jfraction);
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_useAnnealing
(JNIEnv *env, jobject obj, jdouble jfraction, jdouble jmax_temperature) {
  Tuner *t = getHandle<Tuner>(env, obj);
  t->UseAnnealing((double)jfraction, (double)jmax_temperature);
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_usePSO
(JNIEnv *env, jobject obj, jdouble jfraction, jint jswarm_size, jdouble jinfluence_global, jdouble jinfluence_local, jdouble jinfluence_random) {
  Tuner *t = getHandle<Tuner>(env, obj);
  t->UsePSO((double)jfraction, (double)jswarm_size, (double)jinfluence_global, (double)jinfluence_local, (double)jinfluence_random);
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_outputSearchLog
(JNIEnv *env, jobject obj, jstring jfilename) {
  const string filename = jstringToString(env, jfilename);
  Tuner *t = getHandle<Tuner>(env, obj);
  t->OutputSearchLog(filename);
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_tune
(JNIEnv *env, jobject obj) {
  Tuner *t = getHandle<Tuner>(env, obj);
  t->Tune();
}

JNIEXPORT jdouble JNICALL Java_ca_uwaterloo_jcltune_JTuner_printToScreen(
    JNIEnv *env, jobject obj) {
  Tuner *t = getHandle<Tuner>(env, obj);
  return (jdouble) t->PrintToScreen();
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_printFormatted
(JNIEnv *env, jobject obj) {
  Tuner *t = getHandle<Tuner>(env, obj);
  t->PrintFormatted();
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_printJSON
(JNIEnv *env, jobject obj, jstring jfilename, jobject jdescriptions) {
  Tuner *t = getHandle<Tuner>(env, obj);
  // TODO
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_printToFile
(JNIEnv *env, jobject obj, jstring jfilename) {
  Tuner *t = getHandle<Tuner>(env, obj);
  t->PrintToFile(jstringToString(env, jfilename));
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_suppressOutput
(JNIEnv *env, jobject obj) {
  Tuner *t = getHandle<Tuner>(env, obj);
  t->SuppressOutput();
}

JNIEXPORT void JNICALL Java_ca_uwaterloo_jcltune_JTuner_dispose
(JNIEnv *env, jobject obj) {
  Tuner *t = getHandle<Tuner>(env, obj);
  delete t;
  t = 0;
  setHandle(env, obj, t);
}
