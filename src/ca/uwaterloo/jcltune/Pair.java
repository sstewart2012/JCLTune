package ca.uwaterloo.jcltune;

/**
 * Generic pair class.
 * 
 * @param <L> Left entry of the pair.
 * @param <R> Right entry of the pair.
 */
public class Pair<L, R> {
  L first;
  R second;

  public Pair(L first, R second) {
    this.first = first;
    this.second = second;
  }
}
