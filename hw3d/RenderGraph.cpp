#include "stdafx.h"
#include "RenderGraph.h"

namespace Rgph
{
	RenderGraph::RenderGraph(Graphics& gfx)
		: backRenderTarget(gfx.GetTarget()),
		  masterDepthStencil(std::make_unique<Bind::OutputOnlyDepthStencil>(gfx))
	{
		// setup global sinks and sources
		AddGlobalSource(DirectBufferSource<Bind::RenderTarget>::Make("backbuffer", backRenderTarget));
		AddGlobalSource(DirectBufferSource<Bind::DepthStencil>::Make("masterDepth", masterDepthStencil));
		AddGlobalSink(DirectBufferSink<Bind::RenderTarget>::Make("backbuffer", backRenderTarget));
	}

	RenderGraph::~RenderGraph()
	{
	}

	void RenderGraph::Execute(Graphics& gfx) noexcpt
	{
		assert(finalized);
		for (auto& p : passes)
		{
			p->Execute(gfx);
		}
	}

	void RenderGraph::Reset() noexcept
	{
		assert(finalized);
		for (auto& p : passes)
		{
			p->Reset();
		}
	}

	RenderQueuePass& RenderGraph::GetRenderQueue(const std::string& passName)
	{
		try
		{
			for (const auto& p : passes)
			{
				if (p->GetName() == passName)
				{
					return dynamic_cast<RenderQueuePass&>(*p);
				}
			}
		}
		catch (std::bad_cast&)
		{
			throw RGC_EXCEPTION("In RenderGraph::GetRenderQueue, pass was not RenderQueuePass: " + passName);
		}
		throw RGC_EXCEPTION("In RenderGraph::GetRenderQueue, pass not found: " + passName);
	}

	void RenderGraph::StoreDepth(Graphics& gfx, const std::string& path) const
	{
		masterDepthStencil->ToSurface(gfx).Save(path);
	}

	void RenderGraph::SetSinkTarget(const std::string& sinkName, const std::string& target)
	{
		const auto finder = [&sinkName](const std::unique_ptr<Sink>& s)
		{
			return s->GetRegisteredName() == sinkName;
		};

		const auto i = std::find_if(globalSinks.begin(), globalSinks.end(), finder);
		if (i == globalSinks.end())
			throw RGC_EXCEPTION("Global sinks doesn't exist: " + sinkName);

		auto targetSplit = String::Split(target, ".");
		if (targetSplit.size() != 2u)
			throw RGC_EXCEPTION("Input target has incorrect format");

		(*i)->SetTarget(targetSplit[0], targetSplit[1]);
	}

	void RenderGraph::AddGlobalSource(std::unique_ptr<Source> source)
	{
		globalSources.push_back(std::move(source));
	}

	void RenderGraph::AddGlobalSink(std::unique_ptr<Sink> sink)
	{
		globalSinks.push_back(std::move(sink));
	}

	void RenderGraph::Finalize()
	{
		assert(!finalized);
		for (const auto& p : passes)
		{
			p->Finalize();
		}
		LinkGlobalSinks();
		finalized = true;
	}

	void RenderGraph::AppendPass(std::unique_ptr<Pass> pass)
	{
		assert(!finalized);
		// validate name uniqueness
		for (const auto& p : passes)
		{
			if (p->GetName() == pass->GetName())
				throw RGC_EXCEPTION("Pass name already exists: " + pass->GetName());
		}

		// link outputs from passes (and global outputs) to pass inputs
		LinkSink(*pass);

		// add to container of pass
		passes.push_back(std::move(pass));
	}

	Pass& RenderGraph::FindPassByName(const std::string& name)
	{
		const auto i = std::find_if(passes.begin(), passes.end(), [&name](auto& p)
		{
			return p->GetName() == name;
		});

		if (i == passes.end())
			throw std::runtime_error("Failed to find pass name");

		return **i;
	}

	void RenderGraph::LinkSink(Pass& pass)
	{
		for (auto& si : pass.GetSinks())
		{
			const auto& inputSourcePassName = si->GetPassName();

			if (inputSourcePassName.empty())
			{
				std::ostringstream oss;
				oss << "In pass named [" << pass.GetName() << "] sink named [" << si->GetRegisteredName() << "] has no target source set.";
				throw RGC_EXCEPTION(oss.str());
			}

			// check wether target source is global
			if (inputSourcePassName == "$")
			{
				bool bound = false;
				for (auto& source : globalSources)
				{
					if (source->GetName() == si->GetOutputName())
					{
						si->Bind(*source);
						bound = true;
						break;
					}
				}

				if (!bound)
				{
					std::ostringstream oss;
					oss << "Output names [" << si->GetOutputName() << "] not found in globals";
					throw RGC_EXCEPTION(oss.str());
				}
			}
			else // find source from within existing passes
			{
				bool bound = false;
				for (auto& exisitingPass : passes)
				{
					if (exisitingPass->GetName() == inputSourcePassName)
					{
						auto& source = exisitingPass->GetSource(si->GetOutputName());
						si->Bind(source);
						bound = true;
						break;
					}
				}

				if (!bound)
				{
					std::ostringstream oss;
					oss << "Pass names [" << si->GetOutputName() << "] not found";
					throw RGC_EXCEPTION(oss.str());
				}
			}
		}
	}

	void RenderGraph::LinkGlobalSinks()
	{
		for (auto& sink : globalSinks)
		{
			const auto& inputSourcePassName = sink->GetPassName();
			for (auto& existingPass : passes)
			{
				if (existingPass->GetName() == inputSourcePassName)
				{
					auto& source = existingPass->GetSource(sink->GetOutputName());
					sink->Bind(source);
					break;
				}
			}
		}
	}
}
