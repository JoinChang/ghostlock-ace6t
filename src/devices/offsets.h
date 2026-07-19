#ifndef OFFSETS_H
#define OFFSETS_H

#include <stdint.h>

struct kernel_offsets {
  const char *uname_r;

  /* Memory layout */
  uint64_t kimage_text_base;
  uint64_t p0_page_offset;
  uint64_t p0_phys_offset;
  uint64_t p0_kernel_phys_load;
  uint64_t kernelsnitch_identity_start, kernelsnitch_identity_end;
  uint64_t direct_map_end;

  /* Global symbols (from kallsyms) */
  uint64_t off_init_task, off_init_cred, off_init_uts_ns, off_empty_zero_page;
  uint64_t off_root_task_group, off_selinux_enforcing, off_kptr_restrict;
  uint64_t off_selinux_blob_sizes, off_security_hook_heads, off_kmalloc_caches;
  uint64_t off_anon_pipe_buf_ops, off_ashmem_misc_fops, off_ashmem_fops;
  uint64_t off_ashmem_ioctl, off_ashmem_compat_ioctl, off_ashmem_mmap;
  uint64_t off_ashmem_open, off_ashmem_release, off_ashmem_show_fdinfo;
  uint64_t off_configfs_read_iter, off_configfs_bin_write_iter;
  uint64_t off_copy_splice_read, off_noop_llseek, off_cap_capable_active;
  uint64_t off_slide_nfulnl_logger, off_slide_loggers_0_1, off_slide_boot_id;

  /* Struct field offsets (from BTF) */
  uint32_t task_usage, task_prio, task_normal_prio, task_sched_task_group;
  uint32_t task_pi_lock, task_pi_waiters, task_pi_top_task, task_pi_blocked_on;
  uint32_t task_pid, task_tgid, task_real_parent, task_atomic_flags;
  uint32_t task_real_cred, task_cred, task_comm, task_tasks, task_seccomp;
  uint32_t mm_owner;
  uint32_t waiter_tree, waiter_pi_tree, waiter_task, waiter_lock;
  uint32_t waiter_wake_state, waiter_prio, waiter_deadline, waiter_ww_ctx;
  uint32_t waiter_pi_tree_prio, waiter_pi_tree_deadline;

#define WV_ZERO       0
#define WV_PRIO       1
#define WV_TASK       2
#define WV_LOCK       3
#define WV_WAKE       4
#define WV_WAKE_PRIO  5
  struct { int8_t word; int8_t value_flag; } pselect_words[20];
  int pselect_waiter_word_shift;
  int owner_chain_default;
  int requeue_fail_ok;
  int route_timeout_ms;

  uint32_t cred_uid, cred_securebits, cred_caps, cred_security;
  uint32_t fops_owner, fops_llseek, fops_read, fops_write;
  uint32_t fops_read_iter, fops_write_iter, fops_ioctl, fops_compat_ioctl;
  uint32_t fops_mmap, fops_open, fops_release, fops_splice_read;
  uint32_t fops_show_fdinfo;
};

#define OFFSETS_ENTRY(uname, ...) { .uname_r = uname, __VA_ARGS__ }

/* Physical memory layout for OnePlus targets whose kernel loads at P0 0xa8000000. */
#define P0_LAYOUT_ONEPLUS_P0_A800 \
  .p0_page_offset=0xffffff8000000000ULL, \
  .p0_phys_offset=0x80000000ULL, \
  .p0_kernel_phys_load=0xa8000000ULL, \
  .kernelsnitch_identity_start=0xffffff8000000000ULL, \
  .kernelsnitch_identity_end=0xffffff8c00000000ULL, \
  .direct_map_end=0xffffff9000000000ULL

/* Physical memory layout used by the 6.1 porting branch. */
#define P0_LAYOUT_SM6650 \
  .p0_page_offset=0xffffff8000000000ULL, \
  .p0_phys_offset=0x80000000ULL, \
  .p0_kernel_phys_load=0x88000000ULL, \
  .kernelsnitch_identity_start=0xffffff8000000000ULL, \
  .kernelsnitch_identity_end=0xffffff8300000000ULL, \
  .direct_map_end=0xffffff8300000000ULL

#define PSELECT_WORDS_6_12 \
  .pselect_words = { \
    { 2, WV_ZERO}, \
    { 3, WV_ZERO}, \
    { 4, WV_ZERO}, \
    { 5, WV_PRIO}, \
    { 6, WV_ZERO}, \
    { 7, WV_ZERO}, \
    { 8, WV_ZERO}, \
    { 9, WV_ZERO}, \
    {10, WV_PRIO}, \
    {11, WV_ZERO}, \
    {12, WV_TASK}, \
    {13, WV_LOCK}, \
    {14, WV_WAKE}, \
    {-1, 0} \
  }

#define PSELECT_WORDS_6_6 \
  .pselect_words = { \
    { 0, WV_ZERO}, \
    { 1, WV_ZERO}, \
    { 2, WV_ZERO}, \
    { 3, WV_PRIO}, \
    { 4, WV_ZERO}, \
    { 5, WV_ZERO}, \
    { 6, WV_ZERO}, \
    { 7, WV_ZERO}, \
    { 8, WV_PRIO}, \
    { 9, WV_ZERO}, \
    {10, WV_TASK}, \
    {11, WV_LOCK}, \
    {12, WV_WAKE}, \
    {13, WV_ZERO}, \
    {-1, 0} \
  }

#define PSELECT_WORDS_6_1 \
  .pselect_words = { \
    { 2, WV_ZERO}, \
    { 3, WV_ZERO}, \
    { 4, WV_ZERO}, \
    { 5, WV_ZERO}, \
    { 6, WV_ZERO}, \
    { 7, WV_ZERO}, \
    { 8, WV_TASK}, \
    { 9, WV_LOCK}, \
    {10, WV_WAKE_PRIO}, \
    {11, WV_ZERO}, \
    {-1, 0} \
  }

#define STRUCT_OFFSETS_6_12 \
  .task_usage=0x40, .task_prio=0x94, .task_normal_prio=0x9C, \
  .task_sched_task_group=0x420, .task_pi_lock=0x9EC, .task_pi_waiters=0xA00, \
  .task_pi_top_task=0xA10, .task_pi_blocked_on=0xA18, \
  .task_pid=0x708, .task_tgid=0x70C, .task_real_parent=0x718, \
  .task_atomic_flags=0x6C8, .task_real_cred=0x8F8, .task_cred=0x900, \
  .task_comm=0x910, .task_tasks=0x638, .task_seccomp=0x9C8, \
  .mm_owner=0x410, \
  .waiter_tree=0x00, .waiter_pi_tree=0x28, .waiter_task=0x50, \
  .waiter_lock=0x58, .waiter_wake_state=0x60, .waiter_prio=0x18, \
  .waiter_deadline=0x20, .waiter_ww_ctx=0x68, \
  .waiter_pi_tree_prio=0x40, .waiter_pi_tree_deadline=0x48, \
  .cred_uid=0x08, .cred_securebits=0x28, .cred_caps=0x30, .cred_security=0x80, \
  .fops_owner=0x00, .fops_llseek=0x10, .fops_read=0x18, .fops_write=0x20, \
  .fops_read_iter=0x28, .fops_write_iter=0x30, .fops_ioctl=0x50, \
  .fops_compat_ioctl=0x58, .fops_mmap=0x60, .fops_open=0x68, \
  .fops_release=0x78, .fops_splice_read=0xB8, .fops_show_fdinfo=0xD8, \
  .pselect_waiter_word_shift=0, \
  .owner_chain_default=1, \
  .requeue_fail_ok=0, \
  .route_timeout_ms=10000, \
  PSELECT_WORDS_6_12

#define STRUCT_OFFSETS_6_6 \
  .task_usage=0x40, .task_prio=0x84, .task_normal_prio=0x8C, \
  .task_sched_task_group=0x348, .task_pi_lock=0x90C, .task_pi_waiters=0x920, \
  .task_pi_top_task=0x930, .task_pi_blocked_on=0x938, \
  .task_pid=0x618, .task_tgid=0x61C, .task_real_parent=0x628, \
  .task_atomic_flags=0x5D8, .task_real_cred=0x818, .task_cred=0x820, \
  .task_comm=0x830, .task_tasks=0x550, .task_seccomp=0x8E8, \
  .mm_owner=0x2B0, \
  .waiter_tree=0x00, .waiter_pi_tree=0x28, .waiter_task=0x50, \
  .waiter_lock=0x58, .waiter_wake_state=0x60, .waiter_prio=0x18, \
  .waiter_deadline=0x20, .waiter_ww_ctx=0x68, \
  .waiter_pi_tree_prio=0x40, .waiter_pi_tree_deadline=0x48, \
  .cred_uid=0x08, .cred_securebits=0x28, .cred_caps=0x30, .cred_security=0x80, \
  .fops_owner=0x00, .fops_llseek=0x08, .fops_read=0x10, .fops_write=0x18, \
  .fops_read_iter=0x20, .fops_write_iter=0x28, .fops_ioctl=0x48, \
  .fops_compat_ioctl=0x50, .fops_mmap=0x58, .fops_open=0x68, \
  .fops_release=0x78, .fops_splice_read=0xB8, .fops_show_fdinfo=0xD8, \
  .pselect_waiter_word_shift=0, \
  .owner_chain_default=1, \
  .requeue_fail_ok=1, \
  .route_timeout_ms=12000, \
  PSELECT_WORDS_6_6

#define STRUCT_OFFSETS_6_1 \
  .task_usage=0x40, .task_prio=0x84, .task_normal_prio=0x8C, \
  .task_sched_task_group=0x348, .task_pi_lock=0x924, .task_pi_waiters=0x938, \
  .task_pi_top_task=0x948, .task_pi_blocked_on=0x950, \
  .task_pid=0x630, .task_tgid=0x634, .task_real_parent=0x640, \
  .task_atomic_flags=0x5F0, .task_real_cred=0x830, .task_cred=0x838, \
  .task_comm=0x848, .task_tasks=0x550, .task_seccomp=0x900, \
  .mm_owner=0x2A0, \
  .waiter_tree=0x00, .waiter_pi_tree=0x18, .waiter_task=0x30, \
  .waiter_lock=0x38, .waiter_wake_state=0x40, .waiter_prio=0x44, \
  .waiter_deadline=0x48, .waiter_ww_ctx=0x50, \
  .waiter_pi_tree_prio=0x44, .waiter_pi_tree_deadline=0x48, \
  .cred_uid=0x04, .cred_securebits=0x24, .cred_caps=0x28, .cred_security=0x78, \
  .fops_owner=0x00, .fops_llseek=0x08, .fops_read=0x10, .fops_write=0x18, \
  .fops_read_iter=0x20, .fops_write_iter=0x28, .fops_ioctl=0x50, \
  .fops_compat_ioctl=0x58, .fops_mmap=0x60, .fops_open=0x70, \
  .fops_release=0x80, .fops_splice_read=0xC8, .fops_show_fdinfo=0xE0, \
  .pselect_waiter_word_shift=3, \
  .owner_chain_default=1, \
  .requeue_fail_ok=0, \
  .route_timeout_ms=10000, \
  PSELECT_WORDS_6_1

static const struct kernel_offsets known_offsets[] = {
  /* Add new devices by creating src/devices/<name>/offsets.h */
#include "ace6t/offsets.h"
#include "op15/offsets.h"
#include "rmx5070/offsets.h"
#include "erhai/offsets.h"
  { .uname_r = NULL }
};

#endif
