PHP_ARG_ENABLE(microhouse, whether to enable microhouse support,
[ --enable-microhouse   Enable microhouse support])

if test "$PHP_MICROHOUSE" = "yes"; then
  AC_DEFINE(HAVE_MICROHOUSE, 1, [Whether you have MicroHouse module library])

  extra_sources=""
  PHP_NEW_EXTENSION(microhouse, microhouse.c benchmark.c controller.c $extra_sources, $ext_shared)
fi
