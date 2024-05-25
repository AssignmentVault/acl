#include <acl/memory/FixedSizeAllocator.hpp>
#include <acl/memory/CoalesceAllocator.hpp>
#include <acl/memory/MemoryManager.hpp>

#include <gtest/gtest.h>

#define _16BYTES    0x000010
#define _32BYTES    0x000020
#define _64BYTES    0x000040
#define _128BYTES   0x000080
#define _256BYTES   0x000100
#define _512BYTES   0x000200

////////////////////////////
// FSA TESTS
///////////////////////////

TEST(FSA, FSA_16_BYTES) {
    acl::FixedSizeAllocator& fsa_alloc = acl::FixedSizeAllocator::Create();
    fsa_alloc.init(_16BYTES);

    int* p = (int*)fsa_alloc.alloc(4 * sizeof(int));
    EXPECT_TRUE(p != nullptr);

    fsa_alloc.free(p);
    fsa_alloc.destroy();
}

TEST(FSA, FSA_32_BYTES) {
    acl::FixedSizeAllocator& fsa_alloc = acl::FixedSizeAllocator::Create();
    fsa_alloc.init(_32BYTES);

    int* p = (int*)fsa_alloc.alloc(8 * sizeof(int));
    EXPECT_TRUE(p != nullptr);

    fsa_alloc.free(p);
    fsa_alloc.destroy();
}

TEST(FSA, FSA_64_BYTES) {
    acl::FixedSizeAllocator& fsa_alloc = acl::FixedSizeAllocator::Create();
    fsa_alloc.init(_64BYTES);

    int* p = (int*)fsa_alloc.alloc(16 * sizeof(int));
    EXPECT_TRUE(p != nullptr);

    fsa_alloc.free(p);
    fsa_alloc.destroy();
}

TEST(FSA, FSA_128_BYTES) {
    acl::FixedSizeAllocator& fsa_alloc = acl::FixedSizeAllocator::Create();
    fsa_alloc.init(_128BYTES);

    int* p = (int*)fsa_alloc.alloc(32 * sizeof(int));
    EXPECT_TRUE(p != nullptr);

    fsa_alloc.free(p);
    fsa_alloc.destroy();
}

TEST(FSA, FSA_256_BYTES) {
    acl::FixedSizeAllocator& fsa_alloc = acl::FixedSizeAllocator::Create();
    fsa_alloc.init(_256BYTES);

    int* p = (int*)fsa_alloc.alloc(64 * sizeof(int));
    EXPECT_TRUE(p != nullptr);

    fsa_alloc.free(p);
    fsa_alloc.destroy();
}

TEST(FSA, FSA_512_BYTES) {
    acl::FixedSizeAllocator& fsa_alloc = acl::FixedSizeAllocator::Create();
    fsa_alloc.init(_512BYTES);

    int* p = (int*)fsa_alloc.alloc(128 * sizeof(int));
    EXPECT_TRUE(p != nullptr);

    fsa_alloc.free(p);
    fsa_alloc.destroy();
}

TEST(FSA, FSA_MEMORY_LEAK)
{
    acl::FixedSizeAllocator& fsa_alloc = acl::FixedSizeAllocator::Create();
    fsa_alloc.init(512);
    int* p = (int*)fsa_alloc.alloc(128 * sizeof(int));

    EXPECT_TRUE(p != nullptr);
    EXPECT_DEATH(fsa_alloc.destroy(), "");
}

////////////////////////////
// COALESCE TESTS
///////////////////////////

TEST(CoalesceAllocator, Allocate)
{
    acl::CoalesceAllocator& allocator = acl::CoalesceAllocator::Create();

    allocator.init();
    void* p = allocator.alloc(2 * 4096);
    allocator.free(p);
    allocator.destroy();
}

//TEST(CoalesceAllocator, SmallBlocks)
//{
//    acl::CoalesceAllocator& allocator = acl::CoalesceAllocator::Create();
//
//    allocator.init();
//    void* p1 = allocator.alloc(128);
//    void* p2 = allocator.alloc(64);
//    EXPECT_TRUE(p1 != nullptr);
//    EXPECT_TRUE(p2 != nullptr);
//    allocator.free(p2);
//    allocator.free(p1);
//    allocator.destroy();
//}

////////////////////////////
// MEMORY MANAGER TESTS
///////////////////////////

//TEST(MemoryManager, SimpleAllocAndFree) {
//    acl::MemoryManager& allocator = acl::MemoryManager::Create();
//    allocator.init();
//
//    void* p[10];
//    for (int i = 1; i <= 10; i++) {
//        p[i - 1] = allocator.alloc(i * i * i * i);
//        EXPECT_TRUE(p[i - 1] != nullptr);
//    }
//    for (int i = 1; i <= 10; i++)
//        allocator.free(p[i - 1]);
//
//    allocator.destroy();
//}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
