/* This source code is part of the Virtual Windows Class Library (VWCL).
VWCL is a public C++ class library, placed in the public domain, and is
open source software. VWCL is not governed by any rules other than these:

1) VWCL may be used in commercial and other applications.

2) VWCL may not be distributed, in source code form, in development related
projects, unless the developer is also a VWCL contributor.

3) VWCL may be used in development related projects in binary, compiled form,
by anyone.

4) VWCL shall remain open source software regardless of distribution method.

5) A proper copyright notice referencing the "VWCL Alliance" must be included
in the application and/or documentation.

6) No company or individual can ever claim ownership of VWCL.

7) VWCL source code may be modified or used as a base for another class
library.

8) Your use of this software forces your agreement to not hold any member,
individual or company, liable for any damages resulting from the use of the
provided source code. You understand the code is provided as-is, with no
warranty expressed or implied by any member of the VWCL Alliance. You use
this code at your own risk.

Primary Author of this source code file:	Michael Miller
											(michmill@microsoft.com)
Other Author(s) of this source code file:	Todd Osborne
											(todd@vwcl.org)
Other Author(s) of this source code file:	Mark Burton
											(Mark.Burton@era.ericsson.se)

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VTHREAD
#define VTHREAD

#include "../vstandard.h"
#include "../numbers/vbitmask.hpp"
#include "../utility/vhandle.hpp"
#include <process.h>

/** VThread is a base class for multithreading operations. A full discussion
of threading is beyond the scope of this classes documentation. One fine
book on threading in general, and Win32 specifically, is "Multithreading
Applications in Win32. The complete guide to threads." by Jim Beveridge and
Robert Wiener, published by Addison-Wesley Developers Press,
ISBN# 0-201-44234-5. This book explains, in clear detail, the problems
threading can introduce, as well as the power they provide if used
effectively. As for this class in general, it provides a simple to use
method that works well for most thread implementations. Basically, you
derive a class from this base class and override the OnEntryPoint() function.
This is where you do all of your real work. When you return from this
function, the thread ends. While processing this function, you can call
ShouldExit() periodically to determine if the thread should exit at the next
convenient time. ShouldExit() will return VTRUE if some other code in your
application calls the End() method. NEVER call the End() method from within
your override on OnEntryPoint(), as this would cause a deadlock condition
where code is waiting on the thread to exit, and your thread is waiting on
End() to return.*/
class VThread
{
public:
	/** Default constructor can initialize the class options. Defaults
	to no options.*/
	VThread(VUINT nOptions = OPTION_NONE)
	{
		m_Options = nOptions;
		ResetMembers();
	}

	/** Virtual destructor verifies the thread has ended before the
	object goes out of scope.*/
	virtual ~VThread()
		{ End(); }

	/** Options that can be used with this class. Default is OPTION_NONE.
	Options can be set though the GetOptions() function, or during
	construction.*/
	enum	{		OPTION_NONE =						0x0000,
					OPTION_DELETE_SELF_ON_THREAD_END =	0x0001,
			};

	/** Begin a thread. If nStackSize is 0, the default stack size will be
	used. pSecurity is a security descriptor for the new thread. It must be
	NULL for Windows 95 and 98. nInitFlag describes how the thread should be
	created. This value can be 0 which starts the new thread running as soon
	as it is created, or CREATE_SUSPEND to create a thread that is initially
	suspended and must be explicitly started by client code calling the
	Resume() function. When a thread is started, either immediately by this
	function is nInitFlag is 0, or later by Resume(), the pure virtual
	function OnEntryPoint() will be called. Classes derived from this
	class must override this function, as this is where all application
	specified work is performed.*/
	VBOOL			Begin(	VUINT		nStackSize = 0,
							VVOIDPTR	pSecurity = NULL,
							VUINT		nInitFlag = 0)
	{
		/* Always verify these are set in case this class is re-used
		after stopping a thread.*/
		ResetMembers();

		#ifdef __BCPLUSPLUS__
			/* BC++ Version.*/
			return	(m_Handle.Set((HANDLE)_beginthreadNT(
				&VThread::InternalEntryPoint,
				nStackSize,
				this,
				pSecurity,
				nInitFlag,
				NULL)))
				? VTRUE : VFALSE;
		#else
			/* MS C++ Version.*/
			return	(m_Handle.Set((HANDLE)_beginthreadex(
				pSecurity,
				nStackSize,
				&VThread::InternalEntryPoint,
				this,
				nInitFlag,
				&m_ThreadID)))
				? VTRUE : VFALSE;
		#endif
	}

   	/** Call from outside of thread scope to have the thread end itself.
	Returns VTRUE if the thread ended. This function should never be called
	from OnEntryPoint(), or anywhere within the same thread as
	OnEntryPoint(). In otherwords, a thread cannot end itself. A thread
	normally ends itself when the override of OnEntryPoint() returns.*/
	VBOOL			End(VDWORD nWaitTimeOut = INFINITE)
	{
		/* If no thread handle, do nothing.*/
		if ( m_Handle.GetHandle() )
		{
			/* If the thread ID is not known, the thread was
			created suspended and never started.*/
			if ( m_nThreadID )
			{
				/* This should never be called from OnEntryPoint(),
				or anywhere within the same thread as OnEntryPoint()!*/
				VASSERT(GetCurrentThreadId() != m_nThreadID)

				/* Set flag so thread knows we want to exit now.*/
				SetExit();

				if ( WaitForSingleObject(	m_Handle.GetHandle(),
											nWaitTimeOut) == WAIT_OBJECT_0 )
					return VTRUE;
			}
			else
				return VTRUE;
		}

		return VFALSE;
	}

	/** Return the thread handle.*/
	HANDLE			GetHandle() const
		{ return m_Handle.GetHandle(); }

	/** Return the thread ID. This will be 0 if the thread was created in
	a suspended state and not yet ran, or when the call to this function
	is made before the thread actually gets started by the operating system.*/
	VUINT			GetID() const
		{ return m_nThreadID; }

	/** Return a reference to the options object used by this class.*/
	VBitmask&		GetOptions() const
		{ return (VBitmask&)m_Options; }

	/** Determine if the thread is running by looking at the thread ID
	and handle.*/
	VBOOL			IsRunning() const
		{ return (m_nThreadID && m_Handle.GetHandle()) ? VTRUE : VFALSE; }

	/** Set the priority value for this specified thread. This value,
	together with the priority class of the thread's process, determines
	the thread's base priority level. On Windows NT, the thread handle must
	have the THREAD_SET_INFORMATION access right associated with it.
	nPriority specifies the priority value for the thread. This parameter
	can be one of the following values:

	THREAD_PRIORITY_ABOVE_NORMAL - Indicates 1 point above normal priority
	for the priority class.

	THREAD_PRIORITY_BELOW_NORMAL - Indicates 1 point below normal priority
	for the priority class.

	THREAD_PRIORITY_HIGHEST - Indicates 2 points above normal priority for
	the priority class.

	THREAD_PRIORITY_IDLE - Indicates a base priority level of 1 for
	IDLE_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS
	processes, and a base priority level of 16 for REALTIME_PRIORITY_CLASS
	processes.

	THREAD_PRIORITY_LOWEST - Indicates 2 points below normal priority for
	the priority class.

	THREAD_PRIORITY_NORMAL - Indicates normal priority for the priority
	class.

	THREAD_PRIORITY_TIME_CRITICAL - Indicates a base priority level of 15
	for IDLE_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS
	processes, and a base priority level of 31 for REALTIME_PRIORITY_CLASS
	processes.

	Every thread has a base priority level determined by the thread's
	priority value and the priority class of its process. The system uses
	the base priority level of all executable threads to determine which
	thread gets the next slice of CPU time. Threads are scheduled in a
	round-robin fashion at each priority level, and only when there are no
	executable threads at a higher level does scheduling of threads at a
	lower level take place. The Priority() function enables setting the
	base priority level of a thread relative to the priority class of its
	process. For example, specifying THREAD_PRIORITY_HIGHEST in a call to
	Priority() for a thread of an IDLE_PRIORITY_CLASS process sets the
	thread's base priority level to 6. For a table that shows the base
	priority levels for each combination of priority class and thread
	priority value, see the PriorityClass() function. For
	IDLE_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, and HIGH_PRIORITY_CLASS
	processes, the system dynamically boosts a thread's base priority level
	when events occur that are important to the thread.
	REALTIME_PRIORITY_CLASS processes do not receive dynamic boosts. All
	threads initially start at THREAD_PRIORITY_NORMAL. Use the
	PriorityClass() functions to get and set the priority class of a
	process. Use the Priority() function to get the priority value of a
	thread. Use the priority class of a process to differentiate between
	applications that are time critical and those that have normal or below
	normal scheduling requirements. Use thread priority values to
	differentiate the relative priorities of the tasks of a process. For
	example, a thread that handles input for a window could have a higher
	priority level than a thread that performs intensive calculations for
	the CPU. When manipulating priorities, be very careful to ensure that a
	high-priority thread does not consume all of the available CPU time. A
	thread with a base priority level above 11 interferes with the normal
	operation of the operating system. Using REALTIME_PRIORITY_CLASS may
	cause disk caches to not flush, hang the mouse, and so on. On Windows
	CE, the thread's priority level is one of the following values:

	THREAD_PRIORITY_TIME_CRITICAL - Indicates 3 points above normal
	priority.

	THREAD_PRIORITY_HIGHEST - Indicates 2 points above normal priority.

	THREAD_PRIORITY_ABOVE_NORMAL - Indicates 1 point above normal priority.

	THREAD_PRIORITY_NORMAL - Indicates normal priority.

	THREAD_PRIORITY_BELOW_NORMAL - Indicates 1 point below normal priority.

	THREAD_PRIORITY_LOWEST - Indicates 2 points below normal priority.

	THREAD_PRIORITY_ABOVE_IDLE - Indicates 3 points below normal priority.

	THREAD_PRIORITY_IDLE - Indicates 4 points below normal priority.

	Windows CE does not support priority classes. The order in which
	threads are scheduled is determined only by their thread priorities.
	When manipulating priorities, ensure that a high-priority thread does
	not consume all of the available CPU time. A thread with a priority
	level of THREAD_PRIORITY_TIME_CRITICAL will execute until it explicitly
	yields processing to other threads. Processing of these threads is not
	yielded to other threads with the THREAD_PRIORITY_TIME_CRITICAL
	priority level. Such a thread can interfere with the normal operation
	of the operating system if the thread does not explicitly yield
	processing quickly.

	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			Priority(VINT nPriority) const
	{
		VASSERT(GetHandle())
		return (SetThreadPriority(GetHandle(), nPriority)) ? VTRUE : VFALSE;
	}

	/** Return the current priority setting of this thread.*/
	VINT			Priority() const
	{
		VASSERT(GetHandle())
		return GetThreadPriority(GetHandle());
	}

	/** Sets the priority class for the specified process, NOT a specific
	thread. This value together with the priority value of each thread of
	the process determines each thread's base priority level. hProcess is
	the handle to the process to set the priority class for. If NULL, the
	current process is used, the one your code is running in. nPriorityClass
	specifies the priority class for the process. Specify one of the
	following values:

	HIGH_PRIORITY_CLASS - Specify this class for a process that performs
	time-critical tasks that must be executed immediately. The threads of
	the process preempt the threads of normal or idle priority class
	processes. An example is the Task List, which must respond quickly when
	called by the user, regardless of the load on the operating system. Use
	extreme care when using the high-priority class, because a high-priority
	class application can use nearly all available CPU time.

	IDLE_PRIORITY_CLASS - Specify this class for a process whose threads run
	only when the system is idle. The threads of the process are preempted
	by the threads of any process running in a higher priority class. An
	example is a screen saver. The idle-priority class is inherited by child
	processes.

	NORMAL_PRIORITY_CLASS - Specify this class for a process with no
	special scheduling needs.

	REALTIME_PRIORITY_CLASS - Specify this class for a process that has the
	highest possible priority. The threads of the process preempt the
	threads of all other processes, including operating system processes
	performing important tasks. For example, a real-time process that
	executes for more than a very brief interval can cause disk caches not
	to flush or cause the mouse to be unresponsive. Every thread has a base
	priority level determined by the thread's priority value and the
	priority class of its process. The system uses the base priority level
	of all executable threads to determine which thread gets the next slice
	of CPU time. The Priority() function enables setting the base priority
	level of a thread relative to the priority class of its process. This
	function returns VTRUE on success, VFALSE on failure.*/
	static VBOOL	PriorityClass(	VDWORD	nPriorityClass,
									HANDLE	hProcess = NULL)
	{
		return (SetPriorityClass(	(hProcess)
									? hProcess
									: GetModuleHandle(NULL),
									nPriorityClass))
									? VTRUE : VFALSE;
	}

	/** Return the current priority class setting for the process in
	hProcess, of this process, if NULL.*/
	static VDWORD	PriorityClass(HANDLE hProcess = NULL)
	{
		return GetPriorityClass(	(hProcess)
									? hProcess
									: GetModuleHandle(NULL));
	}

	/** Decrements a thread's suspend count. When the suspend count is
	decremented to zero, the execution of the thread is resumed. On Windows
	NT, the thread handle must have THREAD_SUSPEND_RESUME access to the
	thread. The Resume() function checks the suspend count of the subject
	thread. If the suspend count is 0, the thread is not currently
	suspended. Otherwise, the subject thread's suspend count is
	decremented. If the resulting value is 0, then the execution of the
	subject thread is resumed. If the return value is 0, the specified
	thread was not suspended. If the return value is 1, the specified
	thread was suspended but was restarted. If the return value is greater
	than 1, the specified thread is still suspended. Note that while
	reporting debug events, all threads within the reporting process are
	frozen. Debuggers are expected to use the Suspend() and Resume()
	functions to limit the set of threads that can execute within a
	process. By suspending all threads in a process except for the one
	reporting a debug event, it is possible to "single step" a single
	thread. The other threads are not released by a continue operation if
	they are suspended. If the function succeeds, the return value is the
	thread's previous suspend count. If the function fails, the return
	value is 0xFFFFFFFF. To get extended error information, call
	GetLastError().*/
	VDWORD			Resume() const
	{
		VASSERT(GetHandle())
		return ResumeThread(GetHandle());
	}

	/** Suspend this thread. On Windows NT, the thread handle must have
	THREAD_SUSPEND_RESUME access. If the function succeeds, execution of the
	specified thread is suspended and the thread's suspend count is
	incremented. Suspending a thread causes the thread to stop executing
	user-mode (application) code. Each thread has a suspend count (with a
	maximum value of MAXIMUM_SUSPEND_COUNT). If the suspend count is greater
	than zero, the thread is suspended, otherwise, the thread is not
	suspended and is eligible for execution. Calling Suspend() causes the
	target thread's suspend count to be incremented. Attempting to increment
	past the maximum suspend count causes an error without incrementing the
	count. The Resume() function decrements the suspend count of a suspended
	thread. If the function succeeds, the return value is the thread's
	previous suspend count; otherwise, it is 0xFFFFFFFF. To get extended
	error information, use the GetLastError() function.*/
	VDWORD			Suspend() const
	{
		VASSERT(GetHandle())
		return SuspendThread(GetHandle());
	}

	/** Forcefully terminate this thread. On Windows NT, the thread handle
	must have THREAD_TERMINATE access. Terminate() is used to cause a
	thread to exit. When this occurs, the target thread has no chance to
	execute any user-mode code and its initial stack is not deallocated.
	DLLs attached to the thread are not notified that the thread is
	terminating. Terminate() is a dangerous function that should only be
	used in the most extreme cases. You should call Terminate() only if you
	know exactly what the target thread is doing, and you control all of
	the code that the target thread could possibly be running at the time
	of the termination. For example, Terminate() can result in the
	following problems:

	1) If the target thread owns a critical section, the critical section
	will not be released.

	2) If the target thread is executing certain kernel32 calls when it is
	terminated, the kernel32 state for the thread's process could be
	inconsistent.

	3) If the target thread is manipulating the global state of a shared
	DLL, the state of the DLL could be destroyed, affecting other users of
	the DLL.

	A thread cannot protect itself against Terminate(), other than by
	controlling access to its handles. The thread handle returned by the
	CreateThread() and CreateProcess() functions has THREAD_TERMINATE
	access, so any caller holding one of these handles can terminate your
	thread. If the target thread is the last thread of a process when this
	function is called, the thread's process is also terminated. The state
	of the thread object becomes signaled, releasing any other threads that
	had been waiting for the thread to terminate. The thread's termination
	status changes from STILL_ACTIVE to the value of the nExitCode
	parameter. Terminating a thread does not necessarily remove the thread
	object from the system. A thread object is deleted when the last thread
	handle is closed. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			Terminate(VDWORD nExitCode = 0)
	{
		/* Skip if no handle.*/
		if ( GetHandle() && TerminateThread(GetHandle(), nExitCode) )
		{
			ResetMembers();
			return VTRUE;
		}

		return VFALSE;
	}

protected:
	/** Pure virtual function must be overridden to use this class.
	Implementation of this function should monitor ShouldExit() to determine
	when the thread should stop. When ShouldExit() returns VTRUE, the
	override of this function should return as soon as possible.*/
	virtual void	OnEntryPoint() = 0;

	/** Called after the thread ends, that is after OnEntryPoint() returns.
	This base class function does nothing.*/
	virtual void	OnThreadEnd()
		{;}

	/** Called to reset internal member variables.*/
	virtual void	ResetMembers()
	{
		m_Handle.Close();
		m_nThreadID =		0;
		m_bExitThread =		VFALSE;
	}

	/** This function is called when the thread processing should end. This
	base class function sets the boolean flag m_bExitThread to VTRUE. More
	complex or sophisticated applications may want to provide a more
	involved version of this function.*/
	virtual void	SetExit()
		{ m_bExitThread = VTRUE; }

	/** The overridden OnEntryPoint() function should call this function to
	determine if the thread should end. This function should only be called
	from the override of OnEntryPoint(), nowhere else. The base class
	version of this function returns VTRUE if the internal boolean
	m_bExitThread is VTRUE. More complex or sophisticated applications may
	want to provide a more involved version of this function.*/
	virtual VBOOL	ShouldExit()
	{
		VASSERT(m_nThreadID && GetCurrentThreadId() == m_nThreadID)
		return m_bExitThread;
	}

	/** Embedded Members.*/
	VBitmask		m_Options;
	VHandle			m_Handle;
	VDWORD			m_nThreadID;
	VBOOL			m_bExitThread;
    VUINT           m_ThreadID;
private:
	/** Real thread entry point used by CRT calls virtual function
	OnEntryPoint() to start thread processing.*/
	static unsigned __stdcall	InternalEntryPoint(void* pVThread)
	{
		VThread* pThread = (VThread*)pVThread;
		VASSERT(pThread)

		/* Set the thread ID.*/
		pThread->m_nThreadID = GetCurrentThreadId();

		/* Call pure virtual function to handle thread processing.*/
		pThread->OnEntryPoint();

		/* Reset local member variables and close thread handle.*/
		pThread->ResetMembers();

		/* Call OnThreadEnd() notification.*/
		pThread->OnThreadEnd();

		/* Should we delete ourselves?*/
		if ( pThread->m_Options.IsSet(OPTION_DELETE_SELF_ON_THREAD_END) )
			VDELETE_NULL(pThread)

		/* End the thread and let CRT clean up.*/
		_endthreadex(0);

		/* This is never reached, but keeps the compiler happy.*/
		return 0;
	}
};

#endif	/* VTHREAD*/
