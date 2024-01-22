BINARIES=manyreleaseaccess namespace0 namespace1 namespace2		\
namespace2a namespace3 namespace4 namespace5 namespace6 namespace7	\
namespace8 namespace9 namespace10 namespace11 namespace12 namespace13	\
namespace14 namespace15 namespace16 namespace17 namespace18		\
namespace19 namespace20 namespace21 namespace22 namespace23		\
namespace24 namespace25 namespace26 namespaceproxy1		\
namespaceproxy2 namespacefail1 namespacefail2 namespacefail3		\
namespacefail4 namespacefail5 namespacefail6 namespacefail7		\
namespacefail8 namespacefail9 namespacefail10 namespacefail11		\
namespacefail12 noflush_fail1		\
noflush_fail1 noflush_fail2 noflush_fail3		\
noflush_fail4 noflush_fail5 noflush_fail6 noflush_fail7 noflush_fail8	\
noflush_fail9 noflush_fail10 noflush_fail11 newfail noflush_fail12	\
test2_memory_consumption

all:${BINARIES}

#With ASAN
#%:%.c
#	mcc --ompss-2  -I/apps/MVAPICH2/2.3rc1/INTEL/include -L/apps/MVAPICH2/2.3rc1/INTEL/lib -Wl,-rpath -Wl,/apps/MVAPICH2/2.3rc1/INTEL/lib -Wl,--enable-new-dtags -fsanitize=address -fno-omit-frame-pointer -ggdb -o $@ $<
#	mcc --ompss-2  -I/apps/MVAPICH2/2.3rc1/INTEL/include -L/apps/MVAPICH2/2.3rc1/INTEL/lib -Wl,-rpath -Wl,/apps/MVAPICH2/2.3rc1/INTEL/lib -Wl,--enable-new-dtags -lmpi -fsanitize=address -fno-omit-frame-pointer -ggdb $@.o -o $@

%:%.c
	mcc --ompss-2  -o $@ $<

clean:
	rm -rf ${BINARIES}
