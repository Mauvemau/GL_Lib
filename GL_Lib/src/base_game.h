#include "deps.h"

namespace gllib {

	class DLLExport BaseGame {
	private:
		void internalInit();
		void internalUpdate();
		void internalUninit();

	protected:
		virtual void init();
		virtual void update();
		virtual void uninit();

	public:
		BaseGame();
		virtual ~BaseGame();

		void start();
	};

}