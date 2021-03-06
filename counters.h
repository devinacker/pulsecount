#ifndef __LINUX_COUNTERS_H
#define __LINUX_COUNTERS_H

#include <linux/spinlock.h>
#include <linux/time.h>

/* Device class name */
#define DEVICE_CLASS "counters"
/* Device base name */
#define DEVICE_NAME  "counter"

/*
 * Counters class device driver common resource
 */
struct counters_device {
    /* Physical resource name */
    const char* name;
    /* Measuremens lock */
    spinlock_t measurements_lock;
    /* Measuremens: detected pulse count */
    unsigned long pulse_count;
    /* Measuremens: last detected pulse timestamp */
    struct timeval last_pulse;
    /* Measuremens: last detected pulse period (us) */
    struct timeval last_pulse_period;
    /* Measuremens: average pulse period (us) */
    struct timeval average_pulse_period;
    /* Release device driver's resources function */
    void (*shutdown)(struct counters_device *);
    /* Kernel device resource */
    struct device dev;
};
/* Retrieve struct counters_device from struct device pointer */
#define to_counters_device(d) container_of(d, struct counters_device, dev)

/**
 * Increment struct device usage counter
 * 
 * @param dev
 * @return 
 */
static inline struct counters_device *counters_get_device(struct counters_device *dev) {
    return dev ? to_counters_device(get_device(&dev->dev)) : NULL;
}

/**
 * Decrement struct device usage counter
 * 
 * @param dev
 */
static inline void counters_put_device(struct counters_device *dev) {
    if(dev)
        put_device(&dev->dev);
}

/*
 * GPIO pulse counter device driver resource
 */
struct gpio_pulse_counter {
    /* GPIO pin IRQ number */
    int irq;
    /* GPIO pin number */
    int gpio;
};

struct counters_device *counters_allocate_device(const char* name, size_t driver_private_data_size);
void counters_free_device(struct counters_device *dev);
int counters_register_device(struct counters_device *dev);
void counters_unregister_device(struct counters_device *dev);
void counters_pulse(struct counters_device *dev);

#endif
