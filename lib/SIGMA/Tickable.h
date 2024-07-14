#ifndef TICKABLE_H
#define TICKABLE_H

/**
 * Base interface for all elements of Controller layer in SIGMA architecture
 */
class Tickable
{
public:
    virtual void tick() = 0;
};

#endif // TICKABLE_H
