// LICENSETEXT
// 
//   Copyright (C) 2007-2008 : GreenSocs Ltd
//       http://www.greensocs.com/ , email: info@greensocs.com
// 
//   Developed by :
// 
//   Wolfgang Klingauf, Robert Guenzel, Christian Schroeder
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
// 
//   Mark Burton, Marcus Bartholomeu
//     GreenSocs Ltd
// 
// 
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#ifndef __gc_memoryutils_h__
#define __gc_memoryutils_h__


//#define BOOST_SP_DISABLE_THREADS
//#include "boost/shared_array.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"


namespace gs {
namespace ctr {
  

  template <class T>
  class ObjectPool{
    struct entry{
      public:
      entry(T* content){
        that=content;
        next=NULL;
      }
      
      T* that;
      entry* next;
    };
    
  public:
    ObjectPool(int size): used(NULL){ // (CS) Warning: potential memory issue when size is big
      unused=new entry(new T());  //create first one
      mine.push_back(unused->that);
      for (int i=0; i<size-1; i++){
        entry* e=new entry(new T());
        e->next=unused;
        unused=e;
        mine.push_back(unused->that);
      }
    }
    
    ~ObjectPool(){
      //delete all T* that belong to this pool
      for (unsigned int i=0; i<mine.size(); i++){
        delete mine[i];
      }
      
      //delete all unused elements
      while (unused){
        entry* e=unused;
        unused=unused->next;
        delete e;
      }

      //delete all used elements
      while (used){
        entry* e=used;
        used=used->next;
        delete e;
      }
    }
    
    bool is_from(T* cont){ //slow!!!
      for (int i=0; i<mine.size(); i++){
        if (mine[i]==cont) return true;
      }
      return false;
    }
    
    T* construct(){
      entry* e;
      if (unused==NULL){
        e=new entry(new T());
        mine.push_back(e->that);
      }
      else{
        e=unused;
        unused=unused->next;
      }
      e->next=used;
      used=e;
      return used->that; //if all elements of pool are used, just create a new one and go on      
    }

    void free (T* cont){
      assert(used);
      entry* e=used;
      used=e->next;
      e->that=cont;
      e->next=unused;
      unused=e;
    }
    
  private:
    entry* unused;
    entry* used;
    std::vector<T*> mine; //just for clean up and is_from
  };


  //---------------------------------------------------------------------------
  /**
   * Abstraction for object creation, plain as can be
   */
  //---------------------------------------------------------------------------
  template <typename T,bool needsRelease=false>
  struct ObjectFactory
  {
    T create() {
      return T();
    }
  };

#define PTRTYPE boost::shared_ptr

  //---------------------------------------------------------------------------
  /**
   * Specialization for shared pointers from a pool, for objects that
   * do not need release
   */
  //---------------------------------------------------------------------------
  template <typename T>
  struct ObjectFactory<PTRTYPE<T>, false>
  {
    ObjectFactory( int poolSize=50/*512*/ ) : // (CS) Warning: potential memory issue when size is 512
      m_pool( poolSize )
      {
        (get_livingFactories())[this]=true;
      };

    ~ObjectFactory(){
      (get_livingFactories())[this]=false; // necessary to avoid segfault at GreenBus deconstruction
    }

    inline PTRTYPE<T> create()
    {
      T* tmp=m_pool.construct(); // ruft constructor auf!
      // return a shared_ptr that calls our release function on deletion
      return PTRTYPE<T>(tmp,
                                  boost::bind(&ObjectFactory<PTRTYPE<T> >::st_release, tmp, this) );
    };
    
    bool is_from(PTRTYPE<T> t) 
      {
        return m_pool.is_from(t.get());
      };
    
  private:

    inline void release( T* p )
    {
      m_pool.free( p );
    }
    ObjectPool<T> m_pool;
    
    
    //this static method is called whenever a T* of factory fact is about to be freed.
    //the method checks whether the factory is still alive and if so frees T, otherwise nothing will happen.
    static void st_release( T* p, ObjectFactory<PTRTYPE<T>, false>* fact)
    {
      if ((get_livingFactories())[fact]){ 
        p->reset();
        fact->release(p);
      }
    }
    
    // getter function to create static function member on demand, to ensure it is available at any time, including static initialization
    static std::map< ObjectFactory<PTRTYPE<T>, false>*, bool>& get_livingFactories() {
      static std::map< ObjectFactory<PTRTYPE<T>, false>*, bool> livingFactories;
      return livingFactories;
    }

  };
  
  // moved to get function get_livingFactories
  //template <typename T>
  //std::map< ObjectFactory<PTRTYPE<T>, false>*, bool> ObjectFactory<PTRTYPE<T>, false>::livingFactories;



  //---------------------------------------------------------------------------
  /**
   * Abstraction for object de-referencing
   */
  //---------------------------------------------------------------------------
  template <typename T>
  struct TypeChar {
    typedef T Type;
  };

  template <typename T>
  struct TypeChar<PTRTYPE<T> > {
    typedef T Type;
  };
      
  template <typename T>
  const T& deref( const T& t ) {
    return t;
  }

  template <typename T>
  const T& deref( const PTRTYPE<T>& sp ) {
    return *sp;
  }

  template <typename T>
  T& deref( T& t ) {
    return t;
  }

  template <typename T>
  T& deref( PTRTYPE<T>& sp ) {
    return *sp;
  }

#undef PTRTYPE


} // end of namespace ctr
} // end of namespace gs


#endif
