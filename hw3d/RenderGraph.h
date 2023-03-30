#pragma once

namespace Rgph
{
	class RenderGraph
	{
	public:
		RenderGraph(Graphics& gfx);
		~RenderGraph();

		void Execute(Graphics& gfx) noexcpt;
		void Reset() noexcept;
		RenderQueuePass& GetRenderQueue(const std::string& passName);

	protected:
		void SetSinkTarget(const std::string& sinkName, const std::string& target);
		void AddGlobalSource(std::unique_ptr<Source> source);
		void AddGlobalSink(std::unique_ptr<Sink> sink);
		void Finalize();
		void AppendPass(std::unique_ptr<Pass> pass);

	private:
		void LinkSink(Pass& pass);
		void LinkGlobalSinks();

		std::vector<std::unique_ptr<Pass>> passes{};
		std::vector<std::unique_ptr<Source>> globalSources;
		std::vector<std::unique_ptr<Sink>> globalSinks;
		std::shared_ptr<Bind::RenderTarget> backRenderTarget;
		std::shared_ptr<Bind::DepthStencil> masterDepthStencil;
		bool finalized = false;
	};
}
