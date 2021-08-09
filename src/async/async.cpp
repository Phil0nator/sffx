
#include "async/async.hpp"


namespace async{

    const char* exception::what() const noexcept 
    { return msg; }

    /**
     * A Coroutine object is able to store a 'function call'
     * with any parameters, of any type.
     * A Coroutine object cannot store a return type, so for situations
     * that require a returned value, run your functions through the EventLoop class'
     * templated execute() functions.
     */

        
    void Coroutine::operator()()
        { fn(); }

    



    /**
     * Create an event loop with a certain number of initial workers.
     * @param workers the number of threads to create initially.
     */
    EventLoop::EventLoop(size_t workers)
    {   
        growThreads(workers);
    }

    EventLoop::~EventLoop(){
        join();
    }

    /**
     * Safely close all workers and await their completion.
     * Once an EventLoop is 'joined' it cannot be used again. 
     */
    void EventLoop::join(){
        isActive = false;
        for(auto& t: threadpool){
            t.second.join();
        }
        threadpool.clear();
    }

    /**
     * Safely reset the number of threads. If the number given is
     * less than the current number of threads, threads will be safely
     * closed until the given number remains. If the number given is greater than the
     * current number of threads, more will be added.
     * @param newsize the number of threads that should be running in this EventLoop
     */
    void EventLoop::resize(size_t newsize){
        if (newsize == threadpool.size()) return;
        if (newsize < threadpool.size()) {
            // shrink
            size_t difference = threadpool.size()-newsize;
            size_t deletables = difference;
            for (;difference!=-1ULL;difference--){
                launch(ThreadTerminator::terminate);
            }
            
            while (threadDeleter.size() < deletables){

            }
            while(threadDeleter.size()){
                auto id = threadDeleter.safeget();
                if (id){
                    auto iter = threadpool.find(*id);
                    iter->second.join();
                    threadpool.erase(iter);
                }
            }

            


        }else{
            if (newsize > max_threads){
                max_threads = newsize;
            }
            // grow
            growThreads(newsize-threadpool.size());
        }

    }

    void EventLoop::checkup(){
        if (activeThreads == threadpool.size()) // if all threads are occupied
        {
            if (threadpool.size() < max_threads) // if more threads are allowed by the user
                growThreads(1);
        }
    }

        
        

    /**
     * Execute a coroutine through this EventLoop.
     * Note: The given coroutine will be ran in a different thread.
     * @param fn a coroutine to be ran asynchronously
     * @returns A Promise that will satisfy when your coroutine finishes.
     * Note: Do NOT allow this promise to go out of scope before the completion
     * of your coroutine. If the promise does not exist when the coroutine
     * finishes, it will result in undefined behavior. To prevent this,
     * make sure you use some blocking function on your promises before they go out of scope.
     * For example, await(), or block_then()
     */
    [[nodiscard]] Promise<bool> EventLoop::execute ( std::function<void()> fn )
        { return execute(Coroutine(fn)); }

    
    /**
     * Execute a coroutine through this EventLoop.
     * Note: The given coroutine will be ran in a different thread.
     * @param coroutine a coroutine to be ran asynchronously
     * @returns A Promise set the return value of your coroutine.
     * Note: Do NOT allow this promise to go out of scope before the completion
     * of your coroutine. If the promise does not exist when the coroutine
     * finishes, it will result in undefined behavior. To prevent this,
     * make sure you use some blocking function on your promises before they go out of scope.
     * For example, await(), or block_then()
     */
    [[nodiscard]] Promise<bool> EventLoop::execute( Coroutine c )
        { return execute<bool>( [=](){ if (c.fn) c.fn(); return true;} ); }

    /**
     * Execute a series of Coroutines through this EventLoop.
     * Note: Each of these coroutines will be ran in a different 
     * thread than the main thread.
     * @param cs a vector of coroutines to be ran asynchronously
     * @returns A promise that will satisfy once ALL of the given coroutines
     * have finished.
     * Note: Do NOT allow this promise to go out of scope before the completion
     * of your coroutine. If the promise does not exist when the coroutine
     * finishes, it will result in undefined behavior. To prevent this,
     * make sure you use some blocking function on your promises before they go out of scope.
     * For example, await(), or block_then()
     */
    [[nodiscard]] Promise<bool> EventLoop::executes( std::vector<Coroutine> cs )
    {
        std::vector<Promise<bool> > promises;
        promises.reserve(cs.size());
        for (auto& c : cs){
            promises.push_back(execute(c));
        }
        return execute( [promises](){ for (auto& p : promises) p->await(); } );
    }

    /**
     * Launch a detached coroutine.
     * You will have no built-in way of knowing when this coroutine will finish,
     * or what it did. However, you do not have to await any promises.
     * @param c a coroutine to detach and launch
     */ 
    void EventLoop::launch( Coroutine c )
        { checkup(); task_handle->enqueue(c.fn); }

        


    void worker_mainloop(std::weak_ptr< Queue< std::function<void (void)> > > activeSignal, EventLoop* rawParent, EventLoop::threadid id){
        while(!activeSignal.expired()){
            if (rawParent->isActive == false){
                return; // exit thread;
            }
            std::optional<std::function<void ()>> maybeTask;
            async::EventLoop::task_handle_t tasks;
            {
                tasks = activeSignal.lock();
            }
            maybeTask = tasks->waitget();
            if (maybeTask) {
                rawParent->activeThreads++;
                
                try{
                    (*maybeTask)();
                } catch ( const ThreadTerminator& t ){
                    rawParent->activeThreads--;
                    rawParent->threadDeleter.enqueue(id);
                    return; // Terminated thread
                }
                
                
                rawParent->activeThreads--;
            }

            std::this_thread::yield();
        }
    }

    void EventLoop::growThreads(size_t workers){
        worker_count += workers;
        for(size_t i = 0; i < workers; i++){
            threadpool[threadIdCounter] = std::thread(worker_mainloop, task_handle, this, threadIdCounter);
            //threadpool.push_back(  );
            threadIdCounter++;
        }
    }


    /**
     * Make a join-ed event loop usable again.
     * If this eventloop is already active this has no effect;
     */
    void EventLoop::reActivate()
    {
        isActive = true;
    }



    namespace algorithm{


    }






}
