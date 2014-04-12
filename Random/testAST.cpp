#include <SSVUtils/SSVUtils.hpp>

namespace Eng
{
	template<typename TTokenType, typename TTokenData, typename TASTType> struct LangSpec
	{
		using TokenType = TTokenType;
		using TokenData	= TTokenData;
		using ASTType 	= TASTType;
	};

	template<typename TL> using TokenType 	= typename TL::TokenType;
	template<typename TL> using TokenData 	= typename TL::TokenData;
	template<typename TL> using ASTType 	= typename TL::ASTType;

	template<typename TL> class Token
	{
		private:
			TokenType<TL> type;
			TokenData<TL> data;

		public:
			inline Token(const Token&) = default;
			inline Token(Token&&) = default;
			inline Token(const TokenType<TL>& mType) noexcept : type{mType} { }
			inline Token(const TokenType<TL>& mType, const TokenData<TL>& mData) : Token{mType}, data{mData} { }

			inline const TokenType<TL>& getType() const noexcept { return type; }
			inline const TokenData<TL>& getData() const noexcept { return data; }
	};

	template<typename TL> class ASTImpl 
	{
		public:
			inline virtual ~ASTImpl() { }
	};

	template<typename TL> class ASTTokenNodeImpl : public ASTImpl<TL>
	{
		private:
			Token<TL> token;

		public:
			inline ASTTokenNodeImpl(Token<TL> mToken) : token{std::move(mToken)} { }
	};

	template<typename TL> class ASTNode
	{
		private:
			ASTType<TL> type;
			ssvu::Uptr<ASTImpl<TL>> impl;
			ASTNode<TL>* parent{nullptr};
			std::vector<ssvu::Uptr<ASTNode<TL>>> children;

		public:
			inline ASTNode() = default;
			inline ASTNode(const ASTNode&) = delete;
			inline ASTNode(ASTNode&&) = default;

			inline void setType(ASTType<TL> mType) { type = mType; }
			template<typename T> inline void setImpl(T mImpl) { impl = std::move(mImpl); }

			inline const ASTType<TL>& getType() const noexcept { return type; }

			inline void emplaceChild(ASTNode<TL> mNode)
			{
				auto& result(ssvu::getEmplaceUptr<ASTNode<TL>>(children, std::move(mNode)));
				result.parent = this;				
			}	

			inline ASTImpl<TL>* getImpl() noexcept 				{ return impl.get(); }
			inline ASTNode<TL>* getParent() noexcept 			{ return parent; }
			inline decltype(children)& getChildren() noexcept 	{ return children; }

			template<typename T> T& getImplAs() noexcept 
			{
				SSVU_ASSERT(impl != nullptr);
				return *reinterpret_cast<T*>(impl.get());
			}
	};

	enum class ParseletType{Token, Node};

	template<typename TL> class ParseletToken;
	template<typename TL> class ParseletNode;

	template<typename TL> class ParseletBase 
	{ 
		public:
			inline virtual ~ParseletBase() { }
			inline virtual bool isToken() { throw; }

			inline ParseletToken<TL>& getAsToken() 
			{ 
				SSVU_ASSERT(isToken());
				return *(reinterpret_cast<ParseletToken<TL>*>(this)); 
			}

			inline ParseletNode<TL>& getAsNode() 
			{ 
				SSVU_ASSERT(!isToken());
				return *(reinterpret_cast<ParseletNode<TL>*>(this)); 
			}
	};
	
	template<typename TL> class ParseletToken : public ParseletBase<TL>
	{ 	
		private: 
			Token<TL> token;

		public:
			inline ParseletToken(Token<TL> mToken) : token{std::move(mToken)} { }
			inline decltype(token)& getToken() { return token; }
			inline bool isToken() override { return true; }
	};
	
	template<typename TL> class ParseletNode : public ParseletBase<TL>
	{ 
		private: 
			ASTNode<TL> node;

		public:
			inline ParseletNode(ASTNode<TL> mNode) : node{std::move(mNode)} { }
			inline decltype(node)& getNode() { return node; }		
			inline bool isToken() override { return false; }
	};

	template<typename TL> using ParseletUptrVec = std::vector<ssvu::Uptr<ParseletBase<TL>>>;

	/*
	template<typename TL> union ParseletUnion
	{
		Token<TL> token;
		ASTNode<TL> node;

		inline ParseletUnion(Token<TL> mToken) : token{std::move(mToken)} { }
		inline ParseletUnion(ASTNode<TL> mNode) : node{std::move(mNode)} { }
		inline ParseletUnion(ParseletUnion&&) = default;

		inline ~ParseletUnion() { }
	};

	template<typename TL> class Parselet
	{
		private:
			ParseletType type;
			ParseletUnion<TL> pu;

		public:
			inline Parselet(Token<TL> mToken) noexcept : type{ParseletType::Token}, pu{std::move(mToken)} { } 
			inline Parselet(ASTNode<TL> mNode) noexcept : type{ParseletType::Node}, pu{std::move(mNode)} { }
			inline Parselet(Parselet&& mOther) noexcept : type{std::move(mOther.type)}, pu{std::move(mOther.pu)} { }

			inline ParseletType getType() const noexcept	{ return type; }
			inline decltype(pu.token)& getToken() noexcept 	{ SSVU_ASSERT(type == ParseletType::Token); return pu.token; }
			inline decltype(pu.node)& getNode() noexcept 	{ SSVU_ASSERT(type == ParseletType::Node); return pu.node; }
	};

	template<typename TL> using Parselet = variant<Token<TL>, ASTNode<TL>>;
	*/

	template<typename TL> class RulePart
	{
		private:
			ParseletType type;
			union
			{
				TokenType<TL> tokenType;
				ASTType<TL> astType;
			};

		public:
			inline RulePart(TokenType<TL> mTokenType) noexcept : type{ParseletType::Token}, tokenType{mTokenType} { } 
			inline RulePart(ASTType<TL> mASTType) noexcept : type{ParseletType::Node}, astType{mASTType} { }

			inline ParseletType getType() const noexcept 					{ return type; }
			inline const decltype(tokenType)& getTokenType() const noexcept	{ SSVU_ASSERT(type == ParseletType::Token); return tokenType; }
			inline const decltype(astType)& getASTType() const noexcept 	{ SSVU_ASSERT(type == ParseletType::Token); return astType; }

			inline bool operator==(const RulePart<TL>& mRhs) const noexcept
			{
				if(type != mRhs.type) return false;
				if(type == ParseletType::Token && tokenType == mRhs.tokenType) return true;
				if(type == ParseletType::Node && astType == mRhs.astType) return true;
				return false;
			}
			inline bool operator!=(const RulePart<TL>& mRhs) const noexcept { return !this->operator==(mRhs); }
	};

	template<typename TL> class RuleKey
	{
		private:
			std::vector<RulePart<TL>> ruleParts;

			template<typename T> inline void ctorImpl(T mP) { ruleParts.emplace_back(mP); }
			template<typename T1, typename T2, typename... TArgs> inline void ctorImpl(T1 mP1, T2 mP2, TArgs... mArgs) 
			{ 
				ctorImpl(mP1); ctorImpl(mP2, mArgs...); 
			}

			template<std::size_t TIdx> inline bool matchesImpl(const RulePart<TL>& mPart) const { return mPart == ruleParts.at(TIdx); }
			template<std::size_t TIdx, typename T1, typename T2, typename... TArgs> inline bool matchesImpl(T1 mP1, T2 mP2, TArgs... mArgs) const
			{	
				return !matchesImpl<TIdx>(mP1) ? false : matchesImpl<TIdx + 1>(mP2, mArgs...);
			}
		
		public:
			template<typename... TArgs> inline RuleKey(TArgs... mArgs)
			{
				ctorImpl(mArgs...);
			}

			template<typename... TArgs> inline bool matches(TArgs... mArgs)
			{			
				return sizeof...(TArgs) != ruleParts.size() ? false : matchesImpl<0>(mArgs...);
			}

			inline std::size_t getSize() const noexcept { return ruleParts.size(); }

			inline const RulePart<TL>& getPartAt(std::size_t mIdx) const noexcept 
			{ 
				SSVU_ASSERT(ruleParts.size() > mIdx);
				return ruleParts[mIdx]; 
			}
	};

	template<typename TL> class Rule
	{
		private:
			RuleKey<TL> ruleKey;
			ASTType<TL> result;
			ssvu::Func<ssvu::Uptr<ASTImpl<TL>>(const ParseletUptrVec<TL>&)> func;

		public:
			inline Rule(RuleKey<TL> mKey, ASTType<TL> mResult, const decltype(func)& mFunc) : ruleKey{std::move(mKey)}, result{mResult}, func{mFunc} { }
			
			inline ASTType<TL> getResult() const noexcept { return result; }
			inline std::size_t getSize() const noexcept { return ruleKey.getSize(); }
			inline const RulePart<TL>& getPartAt(std::size_t mIdx) const noexcept { return ruleKey.getPartAt(mIdx); }
			inline decltype(func)& getFunc() noexcept { return func; }
	};

	template<typename TL> class RuleSet
	{
		private:
			std::vector<ssvu::Uptr<Rule<TL>>> rules;
	
		public:
			template<typename... TArgs> inline Rule<TL>& createRule(TArgs&&... mArgs)
			{
				return ssvu::getEmplaceUptr<Rule<TL>>(rules, std::forward<TArgs>(mArgs)...);
			}

			inline const decltype(rules)& getRules() const noexcept { return rules; }
	};

	template<typename TL> class Parser
	{
		private:
			RuleSet<TL> ruleSet;
			std::vector<Token<TL>> sourceStack;
			ParseletUptrVec<TL> parseStack, nodeStack;

		public:
			inline decltype(ruleSet)& getRuleSet() noexcept { return ruleSet; }		

			inline void shift()
			{
				SSVU_ASSERT(!sourceStack.empty());

				// Take top of source stack
				auto tkn(sourceStack.back());

				// Push it both on parse and node stacks
				ssvu::getEmplaceUptr<ParseletToken<TL>>(parseStack, tkn);
				ssvu::getEmplaceUptr<ParseletToken<TL>>(nodeStack, tkn);				

				// Pop from source stack
				sourceStack.pop_back();
			}

			inline bool parseStackMatchesRule(std::size_t mStartAt, const Rule<TL>& mRule)
			{
				const auto& expSize(mRule.getSize());
				if(expSize + mStartAt > parseStack.size()) return false;
				
				for(auto i(0u); i < expSize; ++i) 
				{
					auto& parseletToCheck(parseStack[mStartAt + i]);

					if(parseletToCheck->isToken())
					{	
						auto& pToken(parseletToCheck->getAsToken());
						if(mRule.getPartAt(i).getTokenType() != pToken.getToken().getType()) return false;
					}
					else
					{	
						auto& pNode(parseletToCheck->getAsNode());
						if(mRule.getPartAt(i).getASTType() != pNode.getNode().getType()) return false;
					}	
				}

				return true;
			}

			inline void makeReduction(const Rule<TL>& mRule)
			{
				ParseletUptrVec<TL> removedNodes;
				for(auto k(0u); k < mRule.getSize(); ++k)
				{
					removedNodes.emplace_back(std::move(nodeStack.back()));
					nodeStack.pop_back();							
				}
				 
				// Create new node for reduction
				ASTNode<TL> newNode;
				newNode.setType(mRule.getResult());
				
				// Add removed nodes as children
				for(auto& n : removedNodes) 
				{
					ASTNode<TL> child;

					if(n->isToken())
					{
						child.setImpl(std::move(std::make_unique<ASTTokenNodeImpl<TL>>(n->getAsToken().getToken())));
					}
					
					newNode.emplaceChild(std::move(child));
				}
				

				nodeStack.emplace_back(std::move(std::make_unique<ParseletNode<TL>>(std::move(newNode))));
			}

			inline void reduceRecursively()
			{
				for(const auto& r : ruleSet.getRules())
				{
					//ssvu::lo("matches?") << r.getParselets() << std::endl;

					for(auto i(0u); i < parseStack.size(); ++i)
					{
						if(!parseStackMatchesRule(i, *r)) continue;
						
						ssvu::lo("matches!") << "yes!" << std::endl;

						// Remove base nodes from parse stack and substitue them with exapansion
						ParseletUptrVec<TL> usedParselets;

						for(auto i(0u); i < r->getSize(); ++i) 
						{
							usedParselets.emplace(std::begin(usedParselets), std::move(parseStack.back()));
							parseStack.pop_back();
						}
						
						ASTNode<TL> reductionResult;
						reductionResult.setType(r->getResult());
						reductionResult.setImpl(std::move(r->getFunc()(usedParselets)));						
						parseStack.emplace_back(std::move(std::make_unique<ParseletNode<TL>>(std::move(reductionResult)))); 	

						// Pop N nodes from node stack into removedNodes, then try reducing further
						makeReduction(*r); reduceRecursively(); return;						
					}
				}
			}

			inline void run(const std::vector<Token<TL>>& mTokens)
			{	
				// Reset parser state
				sourceStack.clear();
				parseStack.clear();
				nodeStack.clear();

				// Push all tokens on the token stack
				//for(const auto& t : mTokens) sourceStack.emplace(std::begin(sourceStack), t);

				while(!sourceStack.empty())
				{
					shift();
					reduceRecursively();
				}
			}
	};
}

namespace Lang
{
	enum class Tkn 
	{
		Number,
		POpen,
		PClose
	};	

	struct TknData { };

	enum class ASTT
	{
		Expr,
		Number
	};

	using Spec = Eng::LangSpec<Tkn, TknData, ASTT>;

	struct ASTExpr : public Eng::ASTImpl<Spec>
	{	
		inline virtual void eval() { }
	};

	struct ASTNumber : public ASTExpr
	{
		int value;

		inline ASTNumber(int mValue) : value{mValue} { }

		inline void eval() override { }
	};
}

int main() 
{ 
	using namespace Eng;
	using namespace Lang;

	Parser<Spec> parser;
	auto& ruleSet(parser.getRuleSet());

	ruleSet.createRule(RuleKey<Spec>{Tkn::Number}, ASTT::Number, [](const ParseletUptrVec<Spec>& mParselets)
	{
		//return std::make_unique<ASTNumber>(mParselets[0].getToken())
		return std::move(std::make_unique<ASTNumber>(1));
	});
	ruleSet.createRule(RuleKey<Spec>{ASTT::Number}, ASTT::Expr, [](const ParseletUptrVec<Spec>& mParselets)
	{
		//return std::make_unique<ASTNumber>(mParselets[0].getToken())
		return std::move(std::make_unique<ASTExpr>());
	});
	
	//ruleSet.createRule(RuleKey<Spec>{ASTT::Expr, Tkn::Plus, ASTT::Expr}, ASTT::AddOp);

	std::vector<Token<Spec>> src 
	{
		{Tkn::Number}
	};	

	parser.run(src);

	return 0;
}