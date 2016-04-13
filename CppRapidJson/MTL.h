#pragma once
#include <array>
#include <utility>
#include <tuple>
#include <ppltasks.h>
#include <inspectable.h>
#include <roapi.h>
#include <asyncinfo.h>
#include <windows.foundation.h>
#include <windows.foundation.collections.h>
#include <sstream>
#include <iomanip>

namespace MTL
{
	template <typename>
	class ComPtr;

	inline void Check(HRESULT hr);

	namespace Internals
	{
		template <typename Traits>
		class Handle;

#pragma region Cloaked

		template <typename Interface>
		struct Cloaked : Interface {};

#pragma endregion

#pragma region IsCloaked

		template <typename Interface>
		struct IsCloaked : std::false_type { };

		template <typename Interface>
		struct IsCloaked<Cloaked<Interface>> : std::true_type { };

#pragma endregion

#pragma region IsBaseOf

		template <typename TBase, typename T, typename ... Ts>
		struct IsBaseOf : std::conditional<std::is_base_of<TBase, T>::value, IsBaseOf<TBase, Ts...>, std::false_type>::type { };

		template <typename TBase, typename T>
		struct IsBaseOf<TBase, T> : std::is_base_of<TBase, T> { };

#pragma endregion

#pragma region IsSame

		template <typename TLhs, typename TRhs, typename ... Ts>
		struct IsSame : std::conditional<std::is_same<TLhs, TRhs>::value, std::true_type, IsSame<TLhs, Ts...>>::type { };

		template <typename TLhs, typename TRhs>
		struct IsSame<TLhs, TRhs> : std::is_same<TLhs, TRhs> { };

#pragma endregion

#pragma region IsTypeSet

		template <typename T, typename ... Ts>
		struct IsTypeSet : std::conditional<IsSame<T, Ts...>::value, std::false_type, IsTypeSet<Ts...>>::type { };

		template <typename T>
		struct IsTypeSet<T> : std::true_type { };

#pragma endregion

#pragma region InterfaceCounter

		template <typename T, typename ... Ts>
		struct InterfaceCounter
		{
			static constexpr unsigned typesCount = InterfaceCounter<Ts...>::typesCount + static_cast<unsigned>(!IsCloaked<T>::value);
		};

		template <typename T>
		struct InterfaceCounter<T>
		{
			static constexpr unsigned typesCount = static_cast<unsigned>(!IsCloaked<T>::value);
		};

#pragma endregion

#pragma region IidsHolder

		template <typename ... Ts>
		struct IidsHolder
		{
			static GUID* getIids() noexcept
			{
				return (new std::array<GUID, sizeof...(Ts)>{__uuidof(Ts)...})->data();
			}
		};

#pragma endregion

#pragma region CloakedFilter

		template <unsigned Counter, typename ... Ts>
		struct CloakedFilter;

		template <unsigned Counter>
		struct CloakedFilter<Counter> : IidsHolder<> { };

		template <unsigned Counter, typename T, typename ... Ts>
		struct CloakedFilter<Counter, T, Ts...> :
				std::conditional<Counter == sizeof...(Ts) + 1,
								 IidsHolder<T, Ts...>,
								 typename std::conditional<IsCloaked<T>::value, CloakedFilter<Counter, Ts...>, CloakedFilter<Counter + 1, Ts..., T>>::type>::type { };

#pragma endregion

#pragma region IidsExtractor

		template <typename T, typename ... Ts>
		struct IidsExtractor : CloakedFilter<0, T, Ts...> { };

#pragma endregion

#pragma region FunctionTraits

		template <typename TFunction>
		struct FunctionTraits;

		template <typename TResult, typename... TArgs>
		struct FunctionTraits<TResult(TArgs ...)>
		{
			using ReturnType = TResult;
			using TypesTuple = std::tuple<TArgs...>;

			static constexpr size_t arity = sizeof...(TArgs);

			template <size_t TIndex>
			struct Argument
			{
				static_assert(TIndex < arity, "error: invalid parameter index.");
				using type = typename std::tuple_element<TIndex, std::tuple<TArgs...>>::type;
			};
		};

		template <typename TResult, typename... TArgs>
		struct FunctionTraits<TResult(*)(TArgs ...)> : FunctionTraits<TResult(TArgs ...)> {};

		template <typename TClass, typename TResult, typename... TArgs>
		struct FunctionTraits<TResult(TClass::*)(TArgs ...)> : FunctionTraits<TResult(TArgs ...)> {};

		template <typename TClass, typename TResult, typename... TArgs>
		struct FunctionTraits<TResult(TClass::*)(TArgs ...) const> : FunctionTraits<TResult(TArgs ...)> {};

#ifdef _M_IX86
		template <typename TResult, typename... TArgs>
		struct FunctionTraits<TResult(__stdcall *)(TArgs ...)> : FunctionTraits<TResult(TArgs ...)> {};

		template <typename TClass, typename TResult, typename... TArgs>
		struct FunctionTraits<TResult(__stdcall TClass::*)(TArgs ...)> : FunctionTraits<TResult(TArgs ...)> {};

		template <typename TClass, typename TResult, typename... TArgs>
		struct FunctionTraits<TResult(__stdcall TClass::*)(TArgs ...) const> : FunctionTraits<TResult(TArgs ...)> {};
#endif

#pragma endregion 

#pragma region RemoveIUnknown

		template <typename TInterface>
		struct DECLSPEC_NOVTABLE RemoveIUnknown abstract : TInterface
		{
			static_assert(std::is_base_of<IUnknown, TInterface>::value, "TInterface must inherit IUnknown");

			operator TInterface*() noexcept
			{
				return this;
			}

		private:
			STDMETHODIMP_(ULONG) AddRef();
			STDMETHODIMP_(ULONG) Release();
			STDMETHODIMP QueryTInterface(IID, void **);
		};

#pragma endregion

#pragma region ComPtrRef

		template <typename TClass>
		class ComPtrRef final
		{
			friend class ComPtr<TClass>;
		public:

			ComPtrRef(const ComPtrRef &other)
				: _pointer(other._pointer) {}

			ComPtrRef(ComPtrRef &&other)
				: _pointer(other._pointer) {}

			ComPtrRef& operator=(const ComPtrRef &) = delete;

			ComPtrRef& operator=(ComPtrRef &&) = delete;

			template <typename TResult>
			operator TResult**() const noexcept
			{
				return ConvertTo<TResult>();
			}

			operator TClass**() const noexcept
			{
				return _pointer;
			}

			operator void**() const noexcept
			{
				return reinterpret_cast<void**>(_pointer);
			}

		private:
			explicit ComPtrRef(TClass **pointer) noexcept
				: _pointer(pointer) { }

			TClass **_pointer;

			template <typename TResult>
			IUnknown** ConvertTo(typename std::enable_if<std::is_base_of<IUnknown, TResult>::value && std::is_base_of<IInspectable, TResult>::value == false>::type * = nullptr) const noexcept
			{
				return reinterpret_cast<IUnknown**>(_pointer);
			}

			template <typename TResult>
			IInspectable** ConvertTo(typename std::enable_if<std::is_base_of<IInspectable, TResult>::value>::type * = nullptr) const noexcept
			{
				return reinterpret_cast<IInspectable**>(_pointer);
			}
		};

#pragma endregion

#pragma region HandleRef

		template <typename Traits>
		class HandleRef final
		{
		public:
			using Pointer = typename Traits::Pointer;

			explicit HandleRef(Handle<Traits> &reference) noexcept
				: _reference(reference) {}

			HandleRef(const HandleRef &other) noexcept
				: _reference(other._reference) {}

			HandleRef(HandleRef &&other) noexcept
				: _reference(std::move(other._reference)) { }

			HandleRef& operator=(const HandleRef &) = delete;

			HandleRef& operator=(HandleRef &&) = delete;

			operator Pointer*() noexcept
			{
				return &_reference._pointer;
			}

			operator Handle<Traits>*() noexcept
			{
				return &_reference;
			}

		private:
			Handle<Traits> &_reference;
		};

#pragma endregion

#pragma region Handle

		template <typename Traits>
		class Handle
		{
			friend class HandleRef<Traits>;
		public:
			using Pointer = typename Traits::Pointer;

			explicit Handle(Pointer value = Traits::Invalid()) noexcept
				: _pointer(value) { }

			Handle(const Handle &) = delete;

			Handle& operator=(const Handle &) = delete;

			Handle(Handle &&other) noexcept
				: _pointer(other.Detach()) { }

			Handle& operator=(Handle &&other) noexcept
			{
				if (this != std::addressof(other))
				{
					Attach(other.Detach());
				}

				return *this;
			}

			~Handle() noexcept
			{
				ReleaseInternal();
			}

			explicit operator bool() const noexcept
			{
				return _pointer != Traits::Invalid();
			}

			Pointer Get() const noexcept
			{
				return _pointer;
			}

			Pointer* GetAddressOf() noexcept
			{
				return &_pointer;
			}

			HandleRef<Traits> operator&() noexcept
			{
				return HandleRef<Traits>(*this);
			}

			void Release() noexcept
			{
				ReleaseInternal();
			}

			Pointer* ReleaseAndGetAddressOf() noexcept
			{
				ReleaseInternal();
				return GetAddressOf();
			}

			void Attach(Pointer pointer) noexcept
			{
				ReleaseInternal();
				_pointer = pointer;
			}

			Pointer Detach() noexcept
			{
				Pointer value = _pointer;
				_pointer = Traits::Invalid();
				return value;
			}

			void Swap(Handle &other) noexcept
			{
				std::swap(_pointer, other._pointer);
			}

		protected:

			void ReleaseInternal() noexcept
			{
				Traits::Release(_pointer);
			}

		private:

			Pointer _pointer;
		};

#pragma endregion

#pragma region AsyncOperationTaskHelper

		template <typename TArgument, bool = std::is_base_of<IUnknown, std::remove_pointer_t<TArgument>>::value>
		struct AsyncOperationTaskHelper;

		template <typename TArgument>
		struct AsyncOperationTaskHelper<TArgument, true> final
		{
			using TResult = ComPtr<std::remove_pointer_t<TArgument>>;
		};

		template <typename TArgument>
		struct AsyncOperationTaskHelper<TArgument, false> final
		{
			using TResult = TArgument;
		};

#pragma endregion

#pragma region AsynOperationHelper

		template <typename TAsyncOperation>
		struct AsynOperationHelper;

		template <typename TArgument>
		struct AsynOperationHelper<ABI::Windows::Foundation::IAsyncOperation<TArgument>> final
		{
			using TOperation = ABI::Windows::Foundation::IAsyncOperation<TArgument>;
			using TResult = typename AsyncOperationTaskHelper<typename ABI::Windows::Foundation::Internal::GetAbiType<typename TOperation::TResult_complex>::type>::TResult;
			using THandler = ABI::Windows::Foundation::IAsyncOperationCompletedHandler<TArgument>;
		};

		template <typename TArgument>
		struct AsynOperationHelper<RemoveIUnknown<ABI::Windows::Foundation::IAsyncOperation<TArgument>>> final
		{
			using TOperation = ABI::Windows::Foundation::IAsyncOperation<TArgument>;
			using TResult = typename AsyncOperationTaskHelper<typename ABI::Windows::Foundation::Internal::GetAbiType<typename TOperation::TResult_complex>::type>::TResult;
			using THandler = ABI::Windows::Foundation::IAsyncOperationCompletedHandler<TArgument>;
		};

		template <typename TArgument, typename TProgress>
		struct AsynOperationHelper<ABI::Windows::Foundation::IAsyncOperationWithProgress<TArgument, TProgress>> final
		{
			using TOperation = ABI::Windows::Foundation::IAsyncOperationWithProgress<TArgument, TProgress>;
			using TResult = typename AsyncOperationTaskHelper<typename ABI::Windows::Foundation::Internal::GetAbiType<typename TOperation::TResult_complex>::type>::TResult;
			using THandler = ABI::Windows::Foundation::IAsyncOperationWithProgressCompletedHandler<TArgument, TProgress>;
		};

		template <typename TArgument, typename TProgress>
		struct AsynOperationHelper<RemoveIUnknown<ABI::Windows::Foundation::IAsyncOperationWithProgress<TArgument, TProgress>>> final
		{
			using TOperation = ABI::Windows::Foundation::IAsyncOperationWithProgress<TArgument, TProgress>;
			using TResult = typename AsyncOperationTaskHelper<typename ABI::Windows::Foundation::Internal::GetAbiType<typename TOperation::TResult_complex>::type>::TResult;
			using THandler = ABI::Windows::Foundation::IAsyncOperationWithProgressCompletedHandler<TArgument, TProgress>;
		};

#pragma endregion
	}

#pragma region ComException

	class ComException final : public std::exception
	{
	public:
		explicit ComException(HRESULT hr) noexcept
			: exception(GetString(hr).data())
			  ,_hr(hr)
		{
#ifdef TEST
			OutputDebugStringA(std::string(exception::what()).append("\r\n").data());
#endif
		}

		ComException(const ComException &other) noexcept
			: _hr(other._hr) {}

		ComException& operator=(const ComException &other) noexcept
		{
			if (this != &other)
			{
				_hr = other._hr;
			}

			return *this;
		}

		HRESULT GetResult() const noexcept
		{
			return _hr;
		}

	private:
		HRESULT _hr;

		static std::string GetString(HRESULT hr) noexcept
		{
			using namespace std;

			stringstream stream;
			stream << "ComException: " << hex << hr;
			return stream.str();
		}
	};

#pragma endregion

#pragma region HStringTraits

	struct HStringTraits final
	{
		using Pointer = HSTRING;

		static Pointer Invalid() noexcept
		{
			return nullptr;
		}

		static void Release(Pointer value) noexcept
		{
			Check(WindowsDeleteString(value));
		}
	};

#pragma endregion

#pragma region HString

	class HString final : public Internals::Handle<HStringTraits>
	{
	public:
		explicit HString(Pointer pointer = HStringTraits::Invalid())
			: Handle<HStringTraits>(pointer) { }

		HString(const wchar_t * const string,
				unsigned const length)
		{
			Check(WindowsCreateString(string,
									  length,
									  GetAddressOf()));
		}

		explicit HString(const wchar_t * const string)
			: HString(string, wcslen(string)) { }

		explicit HString(const std::wstring &string)
			: HString(string.data(), string.size()) { }

		template <unsigned Length>
		explicit HString(const wchar_t (&string)[Length])
			: HString(string, Length - 1) { }

		HString(const HString &other)
		{
			if (HStringTraits::Invalid() != other.Get())
			{
				Check(WindowsDuplicateString(other.Get(),
											 GetAddressOf()));
			}
		}

		HString(HString &&other) noexcept
			: Handle<HStringTraits>(other.Detach()) { }

		HString& operator=(const HString &other)
		{
			if (this != std::addressof(other))
			{
				ReleaseInternal();
				Check(WindowsDuplicateString(other.Get(),
											 GetAddressOf()));
			}
			return *this;
		}

		HString& operator=(HString &&other) noexcept
		{
			if (this != std::addressof(other))
			{
				Attach(other.Detach());
			}
			return *this;
		}

		friend HString operator+(const HString &lhs, const HString &rhs)
		{
			HString result;
			Check(WindowsConcatString(lhs.Get(), rhs.Get(), &result));
			return result;
		}

		friend bool operator==(const HString &lhs, const HString &rhs)
		{
			auto compareResult = 0;
			Check(WindowsCompareStringOrdinal(lhs.Get(),
											  rhs.Get(),
											  &compareResult));
			return compareResult == 0;
		}

		friend bool operator!=(const HString &lhs, const HString &rhs)
		{
			return !(lhs == rhs);
		}

		HString Substring(unsigned start) const
		{
			HString result;
			if (HStringTraits::Invalid() == Get())
			{
				Check(WindowsSubstring(Get(),
									   start,
									   result.GetAddressOf()));
			}
			return result;
		}

		const wchar_t* GetRawBuffer() const noexcept
		{
			if (HStringTraits::Invalid() == Get())
			{
				return nullptr;
			}
			return WindowsGetStringRawBuffer(Get(), nullptr);
		}

		const wchar_t* GetRawBuffer(unsigned *length) const noexcept
		{
			if (HStringTraits::Invalid() == Get())
			{
				return nullptr;
			}
			return WindowsGetStringRawBuffer(Get(), length);
		}

		unsigned Size() const noexcept
		{
			if (HStringTraits::Invalid() == Get())
			{
				return 0;
			}
			return WindowsGetStringLen(Get());
		}

		bool Empty() const noexcept
		{
			return WindowsIsStringEmpty(Get());
		}
	};

#pragma endregion

#pragma region HStringReference

	class HStringReference final
	{
	public:
		HStringReference(const wchar_t * const value,
						 unsigned length)
		{
			Check(WindowsCreateStringReference(value,
											   length,
											   &_stringHeader,
											   &_string));
		}

		explicit HStringReference(const wchar_t * const string)
			: HStringReference(string, wcslen(string)) { }

		template <unsigned Length>
		explicit HStringReference(const wchar_t (&string)[Length])
			: HStringReference(string, Length - 1) { }

		explicit HStringReference(const std::wstring &string)
			: HStringReference(string.data(), string.size()) {}

		HStringReference(const HStringReference &) = delete;

		HStringReference(HStringReference &&other) = delete;

		HStringReference& operator=(const HStringReference &) = delete;

		HStringReference& operator=(HStringReference &&other) = delete;

		void* operator new(size_t) = delete;

		void* operator new[](size_t) = delete;

		void operator delete(void *) = delete;

		void operator delete[](void *) = delete;

		HSTRING Get() const noexcept
		{
			return _string;
		}

	private:
		HSTRING _string = nullptr;
		HSTRING_HEADER _stringHeader = {};
	};

#pragma endregion

#pragma region Module

	struct Module
	{
		static bool CanUnload() noexcept;

		static void Increment() noexcept;

		static void Decrement() noexcept;
	};

#pragma endregion

#pragma region ComClass

	//TODO âûíåñòè ïîääåðæêó ïîäñ÷åòà ññûëîê â îòäåëüíûé êëàññ
	template <typename TDefaultInterface,
			  typename ... TInterfaces>
	class ComClass abstract : public TDefaultInterface,
							  public TInterfaces...
	{
		static_assert(Internals::IsBaseOf<IUnknown, TDefaultInterface, TInterfaces...>::value, "Not all interfaces inherit IUnknown.");
		static_assert(Internals::IsTypeSet<TDefaultInterface, TInterfaces...>::value, "Found duplicate types. You must specify unique types.");

	public:
		STDMETHODIMP_(ULONG) AddRef() noexcept override final
		{
			return InterlockedIncrement(&_counter);
		}

		STDMETHODIMP_(ULONG) Release() noexcept override final
		{
			auto const remaining = InterlockedDecrement(&_counter);
			if (0 == remaining)
			{
				delete this;
			}
			return remaining;
		}

		STDMETHODIMP QueryInterface(const GUID &guid, void **result) noexcept override
		{
			if (guid == __uuidof(TDefaultInterface) ||
				guid == __uuidof(IUnknown) ||
				guid == __uuidof(IInspectable))
			{
				*result = this;
			}
			else
			{
				*result = QueryInterfaceImpl<TInterfaces...>(guid);
			}

			if (nullptr == *result) return E_NOINTERFACE;

			static_cast<IUnknown*>(*result)->AddRef();
			return S_OK;
		}

	protected:
		ComClass() noexcept
		{
			Module::Increment();
		}

		virtual ~ComClass() noexcept
		{
			Module::Decrement();
		}

		template <typename U, typename ... Us>
		void* QueryInterfaceImpl(const GUID &guid) noexcept
		{
			using namespace Internals;

			if (IsCloaked<U>::value || guid != __uuidof(U))
			{
				return QueryInterfaceImpl<Us...>(guid);
			}
			return static_cast<U*>(this);
		}

		template <int = 0>
		void* QueryInterfaceImpl(const GUID &) const noexcept
		{
			return nullptr;
		}

	private:
		volatile ULONG _counter = 1;
	};

#pragma endregion

#pragma region RuntimeClass

	template <typename TDefaultInterface,
			  typename ... TInterfaces>
	class RuntimeClass abstract : public ComClass<TDefaultInterface,
												  TInterfaces...>
	{
	public:
		STDMETHODIMP GetIids(ULONG *count, GUID **array) noexcept override final
		{
			using namespace Internals;

			*count = InterfaceCounter<TDefaultInterface, TInterfaces...>::typesCount;
			*array = const_cast<GUID*>(IidsExtractor<TDefaultInterface, TInterfaces...>::getIids());
			if (nullptr == *array)
			{
				return E_OUTOFMEMORY;
			}
			return S_OK;
		}

		STDMETHODIMP GetTrustLevel(TrustLevel *trustLevel) noexcept override final
		{
			*trustLevel = BaseTrust;
			return S_OK;
		}
	};

#pragma endregion

#pragma region ActivationFactory

	template <typename TClass>
	class ActivationFactory final : public RuntimeClass<IActivationFactory>
	{
	public:
		STDMETHODIMP ActivateInstance(IInspectable **instance) noexcept override final
		{
			*instance = new(std::nothrow) TClass();
			if (nullptr == *instance)
			{
				return E_OUTOFMEMORY;
			}
			return S_OK;
		}

		STDMETHODIMP GetRuntimeClassName(HSTRING *) noexcept override final
		{
			return E_ILLEGAL_METHOD_CALL;
		}
	};

#pragma endregion

#pragma region ComPtr

	template <typename TClass>
	class ComPtr final
	{
		static_assert(std::is_base_of<IUnknown, TClass>::value, "Not all interfaces inherit IUnknown.");

		friend class Internals::ComPtrRef<TClass>;

	public:

		ComPtr() noexcept
			: _pointer(nullptr) { };

		~ComPtr() noexcept
		{
			InternalRelease();
		}

		explicit ComPtr(TClass *defaultInterface) noexcept
			: _pointer(defaultInterface)
		{
			InternalAddRef();
		}

		ComPtr(const ComPtr &other) noexcept
			: ComPtr(other._pointer) { }

		ComPtr(ComPtr &&other) noexcept
			: _pointer(other._pointer)
		{
			other._pointer = nullptr;
		}

		ComPtr& operator=(const ComPtr &other) noexcept
		{
			InternalCopy(other);
			return *this;
		}

		ComPtr& operator=(ComPtr &&other) noexcept
		{
			InternalMove(std::move(other));
			return *this;
		}

		explicit operator bool() const noexcept
		{
			return nullptr != _pointer;
		}

		friend bool operator==(const ComPtr &lhs, const ComPtr &rhs) noexcept
		{
			return lhs._pointer == rhs._pointer;
		}

		friend bool operator!=(const ComPtr &lhs, const ComPtr &rhs) noexcept
		{
			return !(lhs == rhs);
		}

		Internals::RemoveIUnknown<TClass>* operator->() const noexcept
		{
			return Get();
		}

		Internals::ComPtrRef<TClass> operator&() noexcept
		{
			return Internals::ComPtrRef<TClass>(&_pointer);
		}

		Internals::RemoveIUnknown<TClass>* Get() const noexcept
		{
			using namespace Internals;

			return static_cast<RemoveIUnknown<TClass>*>(_pointer);
		}

		TClass** GetAddressOf() noexcept
		{
			return &_pointer;
		}

		void Release() noexcept
		{
			InternalRelease();
		}

		TClass** ReleaseAndGetAddressOf() noexcept
		{
			InternalRelease();
			return GetAddressOf();
		}

		void Attach(TClass *ptr) noexcept
		{
			InternalRelease();
			_pointer = ptr;
			InternalAddRef();
		}

		TClass* Detach() noexcept
		{
			TClass *temp = _pointer;
			_pointer = nullptr;
			return static_cast<TClass*>(temp);
		}

		template <typename U>
		STDMETHODIMP As(Internals::ComPtrRef<U> target) noexcept
		{
			ASSERT(nullptr != _pointer);

			return _pointer->QueryInterface(static_cast<U**>(target));
		}

		void Swap(ComPtr &other) noexcept
		{
			swap(_pointer, other._pointer);
		}

		friend void swap(ComPtr &lhs, ComPtr &rhs) noexcept
		{
			using std::swap;
			swap(lhs._pointer, rhs._pointer);
		}

	private:

		TClass *_pointer = nullptr;

		void InternalAddRef() noexcept
		{
			if (_pointer)
			{
				_pointer->AddRef();
			}
		}

		void InternalRelease() noexcept
		{
			auto temp = _pointer;
			if (temp)
			{
				_pointer = nullptr;
				temp->Release();
			}
		}

		void InternalCopy(const ComPtr &other) noexcept
		{
			if (this != std::addressof(other))
			{
				InternalRelease();
				_pointer = other._pointer;
				InternalAddRef();
			}
		}

		void InternalMove(ComPtr &&other) noexcept
		{
			if (this != std::addressof(other))
			{
				InternalRelease();
				_pointer = other._pointer;
				other._pointer = nullptr;
			}
		}
	};

#pragma endregion

#pragma region IteratorAdapter

	template <class TIterator>
	class IteratorAdapter final : public RuntimeClass<ABI::Windows::Foundation::Collections::IIterator<typename TIterator::value_type>>
	{
		using IteratorType = ABI::Windows::Foundation::Collections::IIterator<typename TIterator::value_type>;

	public:

		IteratorAdapter(TIterator &&begin, TIterator &&end) noexcept
			: _begin(std::forward<TIterator>(begin))
			  , _end(std::forward<TIterator>(end)) { }

		STDMETHODIMP GetRuntimeClassName(HSTRING *className) noexcept override
		{
			*className = HString(IteratorType::z_get_rc_name_impl()).Detach();
			return S_OK;
		}

		STDMETHODIMP get_Current(typename TIterator::value_type *current) noexcept override
		{
			*current = *_begin;
			return S_OK;
		}

		STDMETHODIMP get_HasCurrent(boolean *hasCurrent) noexcept override
		{
			*hasCurrent = _begin != _end;
			return S_OK;
		}

		STDMETHODIMP MoveNext(boolean *hasCurrent) noexcept override
		{
			if (_begin != _end)
			{
				++_begin;
				*hasCurrent = _begin != _end;
			}
			else
			{
				*hasCurrent = false;
			}
			return S_OK;
		}

	private:
		TIterator _begin;
		TIterator _end;
	};

#pragma endregion

#pragma region IterableAdapter

	template <class TCollection>
	class IterableAdapter final : public RuntimeClass<ABI::Windows::Foundation::Collections::IIterable<typename TCollection::value_type>>
	{
		using IterableType = ABI::Windows::Foundation::Collections::IIterable<typename TCollection::value_type>;

	public:

		explicit IterableAdapter(TCollection &&collection) noexcept
			: _collection(std::forward<TCollection>(collection)) { }

		STDMETHODIMP GetRuntimeClassName(HSTRING *className) noexcept override
		{
			*className = HString(IterableType::z_get_rc_name_impl()).Detach();
			return S_OK;
		}

		STDMETHODIMP First(ABI::Windows::Foundation::Collections::IIterator<typename TCollection::value_type> **first) override
		{
			*first = new(std::nothrow) IteratorAdapter<typename TCollection::iterator>(std::begin(_collection), std::end(_collection));
			if (nullptr == *first)
			{
				return E_OUTOFMEMORY;
			}
			return S_OK;
		}

	private:
		TCollection _collection;
	};

#pragma endregion

#pragma region Delegate

	template <typename TDelegateInterface,
			  typename TCallback,
			  typename ... TArgs>
	class Delegate final : public ComClass<TDelegateInterface>
	{
	public:
		explicit Delegate(TCallback &&callback) noexcept
			: _callback(std::forward<TCallback>(callback)) { }

		Delegate(const Delegate &other) noexcept
			: _callback(other._callback) { }

		Delegate(Delegate &&other) noexcept
			: _callback(std::move(other._callback)) { }

		Delegate& operator=(const Delegate &other) noexcept
		{
			if (this != &other)
			{
				_callback = other._callback;
			}
			return *this;
		}

		Delegate& operator=(Delegate &&other) noexcept
		{
			if (this != &other)
			{
				_callback = std::move(other._callback);
			}
			return *this;
		}

		STDMETHODIMP Invoke(TArgs ... args) noexcept override
		{
			return _callback(args...);
		}

	private:
		TCallback _callback;
	};

#pragma endregion

#pragma region Iterator

	template <typename TItem>
	class Iterator final
	{
	public:
		Iterator() noexcept { }

		explicit Iterator(ABI::Windows::Foundation::Collections::IIterable<TItem> *iterable)
		{
			boolean hasCurrent;
			Check(iterable->First(&_iterator));
			Check(_iterator->get_HasCurrent(&hasCurrent));
			if (!hasCurrent) _iterator.Release();
		}

		Iterator(const Iterator &other) noexcept
			: _iterator(other._iterator) {}

		Iterator(Iterator &&other) noexcept
			: _iterator(std::move(other._iterator)) {}

		Iterator& operator=(const Iterator &other) noexcept
		{
			if (this != &other)
			{
				_iterator = other._iterator;
			}
			return *this;
		}

		Iterator& operator=(Iterator &&other) noexcept
		{
			if (this != &other)
			{
				_iterator = std::move(other._iterator);
			}
			return *this;
		}

		const TItem& operator*()
		{
			TItem item;
			Check(_iterator->get_Current(&item));
			return item;
		}

		Iterator& operator++()
		{
			boolean hasNext;
			Check(_iterator->MoveNext(&hasNext));
			if (!hasNext) _iterator.Release();
			return *this;
		}

		friend bool operator==(const Iterator &lhs, const Iterator &rhs)
		{
			return lhs._iterator == rhs._iterator;
		}

		friend bool operator!=(const Iterator &lhs, const Iterator &rhs)
		{
			return !(lhs == rhs);
		}

		friend void swap(Iterator &lhs, Iterator &rhs) noexcept
		{
			using std::swap;
			swap(lhs._iterator, rhs._iterator);
		}

	private:
		ComPtr<ABI::Windows::Foundation::Collections::IIterator<TItem>> _iterator;
	};

#pragma endregion

	template <typename TDelegateInterface,
			  typename TCallback,
			  typename ... TArgs>
	inline Delegate<TDelegateInterface, TCallback, TArgs...> ÑreateDelegate(TCallback &&callback,
																			std::tuple<TArgs...>) noexcept
	{
		return Delegate<TDelegateInterface, TCallback, TArgs...>(std::forward<TCallback>(callback));
	}

	template <typename TClass>
	inline ComPtr<TClass> CreateComPtr(TClass *ptr) noexcept
	{
		return ComPtr<TClass>(ptr);
	}

	template <typename TInterface>
	inline STDMETHODIMP GetActivationFactory(HSTRING activatableClassId,
											 Internals::ComPtrRef<TInterface> factory) noexcept
	{
		return RoGetActivationFactory(activatableClassId,
									  __uuidof(TInterface),
									  static_cast<void**>(factory));
	}

	template <typename TDelegateInterface,
			  typename TCallback>
	inline ComPtr<TDelegateInterface> CreateCallback(TCallback &&callback) noexcept
	{
		using namespace Internals;

		static_assert(std::is_base_of<IUnknown, TDelegateInterface>::value && !std::is_base_of<IInspectable, TDelegateInterface>::value, "Delegates objects must be 'IUnknown' base and not 'IInspectable'");

		using TTuple = typename FunctionTraits<decltype(&TDelegateInterface::Invoke)>::TypesTuple;

		auto delegate = ÑreateDelegate<TDelegateInterface>(std::forward<TCallback>(callback),
														   TTuple());

		using TDelegateType = decltype(delegate);

		return ComPtr<TDelegateInterface>(new TDelegateType(std::move(delegate)));
	};

	template <typename TAsyncOperation>
	inline auto GetTask(TAsyncOperation *asyncOperation) ->
	Concurrency::task<typename Internals::AsynOperationHelper<TAsyncOperation>::TResult>
	{
		using namespace Internals;
		using namespace Concurrency;
		using namespace ABI::Windows::Foundation;

		using TAsyncOperationHelper = AsynOperationHelper<TAsyncOperation>;
		using TOperation = typename TAsyncOperationHelper::TOperation;
		using TResult = typename TAsyncOperationHelper::TResult;

		task_completion_event<TResult> taskCompletitionEvent;

		auto asyncOperationPointer = CreateComPtr(static_cast<TOperation*>(asyncOperation));

		auto callback = CreateCallback<typename TAsyncOperationHelper::THandler>(
			[asyncOperationPointer, taskCompletitionEvent]
			(TOperation *operation, AsyncStatus status) mutable ->
			HRESULT
			{
				try
				{
					ComPtr<IAsyncInfo> asyncInfo;
					Check(asyncOperationPointer.As(&asyncInfo));

					switch (status)
					{
						case AsyncStatus::Completed:
							{
								TResult result;
								Check(operation->GetResults(&result));

								taskCompletitionEvent.set(std::move(result));
								break;
							}
						case AsyncStatus::Canceled:
							{
								taskCompletitionEvent.set_exception(task_canceled());
								break;
							}
						case AsyncStatus::Error:
							{
								HRESULT errorCode;
								Check(asyncInfo->get_ErrorCode(&errorCode));

								taskCompletitionEvent.set_exception(ComException(errorCode));
								break;
							}
						case AsyncStatus::Started:
							break;
						default:
							break;
					}

					asyncOperationPointer.Release();
					Check(asyncInfo->Close());
				}
				catch (const ComException &exception)
				{
					taskCompletitionEvent.set_exception(exception);
				}

				return S_OK;
			});

		Check(asyncOperation->put_Completed(callback.Get()));

		return task<TResult>(taskCompletitionEvent);
	}

	inline void Check(HRESULT hr)
	{
		if (IS_ERROR(hr))
		{
			throw ComException(hr);
		}
	}
}

template <typename TItem>
inline MTL::Iterator<TItem> begin(ABI::Windows::Foundation::Collections::IIterable<TItem> *iterable) noexcept
{
	using namespace MTL;
	return Iterator<TItem>(iterable);
}

template <typename TItem>
inline MTL::Iterator<TItem> end(ABI::Windows::Foundation::Collections::IIterable<TItem> *) noexcept
{
	using namespace MTL;
	return Iterator<TItem>();
}

template <typename TKey, typename TValue>
inline MTL::Iterator<ABI::Windows::Foundation::Collections::IKeyValuePair<TKey, TValue>*> begin(ABI::Windows::Foundation::Collections::IMap<TKey, TValue> *map) noexcept
{
	using namespace MTL;
	using namespace ABI::Windows::Foundation::Collections;
	using namespace ABI::Windows::Foundation;

	ComPtr<IIterable<IKeyValuePair<TKey, TValue>*>> iterable;
	map->QueryInterface<IIterable<IKeyValuePair<TKey, TValue>*>>(&iterable);

	return Iterator<IKeyValuePair<TKey, TValue>*>(iterable.Get());
}

template <typename TKey, typename TValue>
inline MTL::Iterator<ABI::Windows::Foundation::Collections::IKeyValuePair<TKey, TValue>*> end(ABI::Windows::Foundation::Collections::IMap<TKey, TValue> *) noexcept
{
	using namespace MTL;
	using namespace ABI::Windows::Foundation::Collections;

	return Iterator<IKeyValuePair<TKey, TValue>*>();
}

template <typename TKey, typename TValue>
inline MTL::Iterator<ABI::Windows::Foundation::Collections::IKeyValuePair<TKey, TValue>*> begin(ABI::Windows::Foundation::Collections::IMapView<TKey, TValue> *mapView) noexcept
{
	using namespace MTL;
	using namespace ABI::Windows::Foundation::Collections;
	using namespace ABI::Windows::Foundation;

	ComPtr<IIterable<IKeyValuePair<TKey, TValue>*>> iterable;
	mapView->QueryInterface<IIterable<IKeyValuePair<TKey, TValue>*>>(&iterable);

	return Iterator<IKeyValuePair<TKey, TValue>*>(iterable.Get());
}

template <typename TKey, typename TValue>
inline MTL::Iterator<ABI::Windows::Foundation::Collections::IKeyValuePair<TKey, TValue>*> end(ABI::Windows::Foundation::Collections::IMapView<TKey, TValue> *) noexcept
{
	using namespace MTL;
	using namespace ABI::Windows::Foundation::Collections;

	return Iterator<IKeyValuePair<TKey, TValue>*>();
}

template <typename TValue>
inline MTL::Iterator<TValue> begin(ABI::Windows::Foundation::Collections::IVector<TValue> *vector) noexcept
{
	using namespace MTL;
	using namespace ABI::Windows::Foundation::Collections;
	using namespace ABI::Windows::Foundation;

	ComPtr<IIterable<TValue>> iterable;
	vector->QueryInterface<IIterable<TValue>>(&iterable);

	return Iterator<TValue>(iterable.Get());
}

template <typename TValue>
inline MTL::Iterator<TValue> end(ABI::Windows::Foundation::Collections::IVector<TValue> *) noexcept
{
	using namespace MTL;

	return Iterator<TValue>();
}

template <typename TValue>
inline MTL::Iterator<TValue> begin(ABI::Windows::Foundation::Collections::IVectorView<TValue> *vectorView) noexcept
{
	using namespace MTL;
	using namespace ABI::Windows::Foundation::Collections;
	using namespace ABI::Windows::Foundation;

	ComPtr<IIterable<TValue>> iterable;
	vectorView->QueryInterface<IIterable<TValue>>(&iterable);

	return Iterator<TValue>(iterable.Get());
}

template <typename TValue>
inline MTL::Iterator<TValue> end(ABI::Windows::Foundation::Collections::IVectorView<TValue> *) noexcept
{
	using namespace MTL;

	return Iterator<TValue>();
}
