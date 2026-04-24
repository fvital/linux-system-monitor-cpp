#ifndef MEMINFO_H
#define MEMINFO_H

#include <fstream>
#include <string>
#include <unordered_map>

class MemInfo
{
public:
    float mem_total;
    float mem_free;
    float mem_available;
    float buffers;
    float cached;
    float swap_cached;
    float active;
    float inactive;
    float active_anon;
    float inactive_anon;
    float active_file;
    float inactive_file;
    float unevictable;
    float mlocked;
    float swap_total;
    float swap_free;
    float dirty;
    float writeback;
    float anon_pages;
    float mapped;
    float shmem;
    float k_reclaimable;
    float slab;
    float s_reclaimable;
    float s_unreclaim;
    float kernel_stack;
    float page_tables;
    float nfs_unstable;
    float bounce;
    float writeback_tmp;
    float commit_limit;
    float committed_as;
    float vmalloc_total;
    float vmalloc_used;
    float vmalloc_chunk;
    float percpu;
    float hardware_corrupted;
    float anon_huge_pages;
    float shmem_huge_pages;
    float shmem_pmd_mapped;
    float file_huge_pages;
    float file_pmd_mapped;
    float cma_total;
    float cma_free;
    float huge_pages_total;
    float huge_pages_free;
    float huge_pages_rsvd;
    float huge_pages_surp;
    float hugepagesize;
    float hugetlb;

    MemInfo() = default;
    MemInfo(std::ifstream &ifs);

private:
    const std::unordered_map<std::string, float *> att_map_{
        {"MemTotal:", &mem_total},
        {"MemFree:", &mem_free},
        {"MemAvailable:", &mem_available},
        {"Buffers:", &buffers},
        {"Cached:", &cached},
        {"SwapCached:", &swap_cached},
        {"Active:", &active},
        {"Inactive:", &inactive},
        {"Active(anon):", &active_anon},
        {"Inactive(anon):", &inactive_anon},
        {"Active(file):", &active_file},
        {"Inactive(file):", &inactive_file},
        {"Unevictable:", &unevictable},
        {"Mlocked:", &mlocked},
        {"SwapTotal:", &swap_total},
        {"SwapFree:", &swap_free},
        {"Dirty:", &dirty},
        {"Writeback:", &writeback},
        {"AnonPages:", &anon_pages},
        {"Mapped:", &mapped},
        {"Shmem:", &shmem},
        {"KReclaimable:", &k_reclaimable},
        {"Slab:", &slab},
        {"SReclaimable:", &s_reclaimable},
        {"SUnreclaim:", &s_unreclaim},
        {"KernelStack:", &kernel_stack},
        {"PageTables:", &page_tables},
        {"NFS_Unstable:", &nfs_unstable},
        {"Bounce:", &bounce},
        {"WritebackTmp:", &writeback_tmp},
        {"CommitLimit:", &commit_limit},
        {"Committed_AS:", &committed_as},
        {"VmallocTotal:", &vmalloc_total},
        {"VmallocUsed:", &vmalloc_used},
        {"VmallocChunk:", &vmalloc_chunk},
        {"Percpu:", &percpu},
        {"HardwareCorrupted:", &hardware_corrupted},
        {"AnonHugePages:", &anon_huge_pages},
        {"ShmemHugePages:", &shmem_huge_pages},
        {"ShmemPmdMapped:", &shmem_pmd_mapped},
        {"FileHugePages:", &file_huge_pages},
        {"FilePmdMapped:", &file_pmd_mapped},
        {"CmaTotal:", &cma_total},
        {"CmaFree:", &cma_free},
        {"HugePages_Total:", &huge_pages_total},
        {"HugePages_Free:", &huge_pages_free},
        {"HugePages_Rsvd:", &huge_pages_rsvd},
        {"HugePages_Surp:", &huge_pages_surp},
        {"Hugepagesize:", &hugepagesize},
        {"Hugetlb:", &hugetlb}};

    friend std::ifstream &operator>>(std::ifstream &filestream, MemInfo &meminfo);
};

std::ifstream &operator>>(std::ifstream &filestream, MemInfo &meminfo);

#endif // MEMINFO_H