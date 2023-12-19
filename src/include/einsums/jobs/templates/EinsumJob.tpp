/**
 * @file EinsumJob.tpp
 *
 * Contains definitions for the EinsumJob class
 */

#pragma once

#include "einsums/_Common.hpp"

#include "einsums/TensorAlgebraThreads.hpp"
#include "einsums/jobs/EinsumJob.hpp"
#include "einsums/jobs/Job.hpp"
#include "einsums/jobs/ThreadPool.hpp"

#include <atomic>
#include <thread>
#include <type_traits>

BEGIN_EINSUMS_NAMESPACE_HPP(einsums::jobs)

/**
 * @def template_def
 *
 * The template definition for the EinsumJob class is long. This macro replaces the template line so that writing code is not tedious.
 * Deleted at the end of the file. Not valid outside this file.
 */
#define template_def                                                                                                                       \
    template <typename AType, typename ABDataType, typename BType, typename CType, typename CDataType, typename CIndices,                  \
              typename AIndices, typename BIndices>

/**
 * @def einsum_job
 *
 * The template specialization of EinsumJob is long. This macro replaces the class name and template parameters to make coding easier.
 * Deleted at the end of the file. Not valid outside this file.
 */
#define einsum_job EinsumJob<AType, ABDataType, BType, CType, CDataType, CIndices, AIndices, BIndices>

template_def einsum_job::EinsumJob(CDataType C_prefactor, const CIndices &Cs, std::shared_ptr<WritePromise<CType>> C,
                                   const ABDataType AB_prefactor, const AIndices &As, std::shared_ptr<ReadPromise<AType>> A,
                                   const BIndices &Bs, std::shared_ptr<ReadPromise<BType>> B, int num_threads_param, bool is_limit_hard)
    : Job(), _C_prefactor(C_prefactor), _AB_prefactor(AB_prefactor), _Cs(Cs), _As(As), _Bs(Bs), __num_threads(num_threads_param),
      __hard_limit(is_limit_hard) {
    _A    = A;
    _B    = B;
    _C    = C;
    work  = new ABDataType[__num_threads];
    synch = 0;
}

template_def einsum_job::~EinsumJob() {
    delete[] work;
}

template_def void einsum_job::run() {
    std::atomic_thread_fence(std::memory_order_acq_rel);
    auto A = _A->get();
    auto B = _B->get();
    auto C = _C->get();

    this->set_state(detail::RUNNING);

    einsums::tensor_algebra::threads::einsum(_C_prefactor, _Cs, C.get(), _AB_prefactor, _As, *A, _Bs, *B,
                                             ThreadPool::get_singleton().compute_threads(std::this_thread::get_id()),
                                             ThreadPool::get_singleton().index(std::this_thread::get_id()), work, synch);

    _C->release();
    _A->release();
    _B->release();

    this->set_state(detail::FINISHED);

    std::atomic_thread_fence(std::memory_order_acq_rel);
}

#undef einsum_job
#undef template_def
END_EINSUMS_NAMESPACE_HPP(einsums::jobs)