#ifndef BLOCK_LIBRARY_H
#define BLOCK_LIBRARY_H

#define LET(var_declaration) const auto var_declaration;

template <typename T> class Integrator
{
  private:
    T Ts;
    T I;

  public:
    const T &out;
    Integrator(T _Ts) : out(I)
    {
        Ts = _Ts;
        I = 0;
    }
    void tick(T in)
    {
        I += in * Ts;
    }
};

template <typename T> class IntegratorNoTs
{
  private:
    T I;

  public:
    const T &out;
    IntegratorNoTs() : out(I)
    {
        I = 0;
    }
    void tick(T in)
    {
        I += in;
    }
};

template <typename T> class Aperiodic
{
  private:
    T TimeConstant;
    Integrator<T> I;

  public:
    const T &out;
    Aperiodic(T _TimeConstant, T _Ts) : I(_Ts), out(I.out)
    {
        TimeConstant = _TimeConstant;
    }
    void tick(T in)
    {
        LET(err = in - I.out)
        LET(I_in = err / TimeConstant)
        I.tick(I_in);
    }
};

#endif // BLOCK_LIBRARY_H
