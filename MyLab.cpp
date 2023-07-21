
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <vector>
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;



namespace {

  struct OmniPass : public BasicBlockPass {
    static char ID; // Pass identification, replacement for typeid
    OmniPass() : BasicBlockPass(ID) {}

    uint64_t MBAOperation(int value) {

      uint64_t  res = 0;

      res = value + 10;
      res = res + 14;
      res = res ^ 1337;
      return res;
    }

    Instruction *AddBinaryOperatorMBALinearOperation(Instruction &Inst, int lhsOperand, Value *rhsOperand, Type *type){
       /* Constant *Val1 = ConstantInt::get(type, 1337);
        Constant *Val2 = ConstantInt::get(type, 10);
        Constant *Val3 = ConstantInt::get(type, 14);*/

        AllocaInst *Val1 = new AllocaInst(Type::getInt32Ty(Inst.getParent()->getContext()), 0, "Val1", &Inst);
        new StoreInst(ConstantInt::get(Type::getInt32Ty(Inst.getParent()->getContext()), 1337), Val1, &Inst);

        AllocaInst *Val2 = new AllocaInst(Type::getInt32Ty(Inst.getParent()->getContext()), 0, "Val2", &Inst);
        new StoreInst(ConstantInt::get(Type::getInt32Ty(Inst.getParent()->getContext()), 10), Val2, &Inst);

        AllocaInst *Val3 = new AllocaInst(Type::getInt32Ty(Inst.getParent()->getContext()), 0, "Val3", &Inst);
        new StoreInst(ConstantInt::get(Type::getInt32Ty(Inst.getParent()->getContext()), 14), Val3, &Inst);

        Value *Val1VarLoad = new LoadInst(Val1, "Val1VarLoad", &Inst);
        Value *Val2VarLoad = new LoadInst(Val2, "Val2VarLoad", &Inst);
        Value *Val3VarLoad = new LoadInst(Val3, "Val3VarLoad", &Inst);
        Value *CipherVal = ConstantInt::get(Type::getInt64Ty(Inst.getParent()->getContext()), lhsOperand);
        IRBuilder<> Builder(&Inst);

        Value * XorRes = Builder.CreateXor(CipherVal, Val1VarLoad);
        Value * ResSub = Builder.CreateSub(XorRes, Val3VarLoad);
        Value *ResSub2 = Builder.CreateSub(ResSub, Val2VarLoad);
        Instruction *NewInstr = BinaryOperator::CreateAdd(
          ResSub2,
        rhsOperand
      );

      return NewInstr;
    }


    Value *AddStoreMBALinearOperation(Instruction &Inst, int LhsV, ConstantInt *RhsV, Type *type){
       /* Constant *Val1 = ConstantInt::get(type, 1337);
        Constant *Val2 = ConstantInt::get(type, 10);
        Constant *Val3 = ConstantInt::get(type, 14);*/

        IRBuilder<> Builder(&Inst);

        Value *Val1 = Builder.CreateAlloca(Type::getInt32Ty(Inst.getParent()->getContext()));

        Builder.CreateStore(ConstantInt::get(Type::getInt32Ty(Inst.getParent()->getContext()), 1337), Val1);

        Value *Val2 = Builder.CreateAlloca(Type::getInt32Ty(Inst.getParent()->getContext()));

        Builder.CreateStore(ConstantInt::get(Type::getInt32Ty(Inst.getParent()->getContext()), 10), Val2);

        Value *Val3 = Builder.CreateAlloca(Type::getInt32Ty(Inst.getParent()->getContext()));

        Builder.CreateStore(ConstantInt::get(Type::getInt32Ty(Inst.getParent()->getContext()), 14), Val3);


        Val1 = Builder.CreateLoad(Val1);
        Val2 = Builder.CreateLoad(Val2);
        Val3 = Builder.CreateLoad(Val3);
        Value *C = ConstantInt::get(Type::getInt32Ty(Inst.getParent()->getContext()), LhsV);
        errs() << "Valeur dans la fonction MBA linaire: " << LhsV <<"\n";

        Value * XorRes = Builder.CreateXor(C, Val1);
        Value * ResSub = Builder.CreateSub(XorRes, Val3);
        Value *ResSub2 = Builder.CreateSub(ResSub, Val2);
        Value *resAlloc = Builder.CreateAlloca(Type::getInt32Ty(Inst.getParent()->getContext()));

        Builder.CreateStore(ResSub2, resAlloc);

        resAlloc = Builder.CreateLoad(resAlloc);

        return resAlloc;

        /*
        AllocaInst *Val1 = new AllocaInst(Type::getInt32Ty(Inst.getParent()->getContext()), 0, "Val1", &Inst);
        new StoreInst(ConstantInt::get(Type::getInt32Ty(Inst.getParent()->getContext()), 1337), Val1, &Inst);

        AllocaInst *Val2 = new AllocaInst(Type::getInt32Ty(Inst.getParent()->getContext()), 0, "Val2", &Inst);
        new StoreInst(ConstantInt::get(Type::getInt32Ty(Inst.getParent()->getContext()), 10), Val2, &Inst);

        AllocaInst *Val3 = new AllocaInst(Type::getInt32Ty(Inst.getParent()->getContext()), 0, "Val3", &Inst);
        new StoreInst(ConstantInt::get(Type::getInt32Ty(Inst.getParent()->getContext()), 14), Val3, &Inst);

        Value *Val1VarLoad = new LoadInst(Val1, "Val1VarLoad", &Inst);
        Value *Val2VarLoad = new LoadInst(Val2, "Val2VarLoad", &Inst);
        Value *Val3VarLoad = new LoadInst(Val3, "Val3VarLoad", &Inst);

        */

    }


    bool runOnBasicBlock(BasicBlock &BB) override {

      errs() << "Running in Basic block\n";

  /*
      for (GlobalVariable &globalVar : BB.getParent()->getParent()->globals()) {

        if (ConstantDataArray *CA = dyn_cast<ConstantDataArray>(globalVar.getInitializer())) {

          if (CA->isString()) {
            errs() << "Num operands string: " << CA->getNumOperands() <<"\n";

            StringRef newStr = "New string value";
            ArrayType* arrType = ArrayType::get(Type::getInt8Ty(BB.getContext()), newStr.size() + 1);
            GlobalVariable* newGlobalVar = new GlobalVariable(*BB.getParent()->getParent(), arrType, true, GlobalValue::PrivateLinkage,ConstantDataArray::getString(BB.getContext(), newStr), "new_variable_name");

            globalVar.replaceAllUsesWith(newGlobalVar);
            globalVar.eraseFromParent();
          }
        }
      }*/

      for (typename BasicBlock::iterator IT = BB.begin(), IE = BB.end(); IT != IE; IT++) {

        Instruction &Inst = *IT;

        IRBuilder<>Builder(&Inst);

        if (BinaryOperator *BinOp = dyn_cast<BinaryOperator>(&Inst)) {

          for (unsigned int i = 0; i < BinOp->getNumOperands(); ++i) {
            if (ConstantInt *CI = dyn_cast<ConstantInt>(BinOp->getOperand(i))) {
              if (CI->getBitWidth() > 32)
                continue;

              if (i % 2) {
                ReplaceInstWithInst(BB.getInstList(), IT, AddBinaryOperatorMBALinearOperation(*BinOp, MBAOperation(CI->getValue().getLimitedValue()), BinOp->getOperand(1), BinOp->getOperand(i)->getType()));
              } else {
                ReplaceInstWithInst(BB.getInstList(), IT, AddBinaryOperatorMBALinearOperation(*BinOp, MBAOperation(CI->getValue().getLimitedValue()), BinOp->getOperand(0), BinOp->getOperand(i)->getType()));
              }
            }
          }
        } else if (StoreInst *SI = dyn_cast<StoreInst>(&Inst)) {
          errs() << "Value detected: " << SI->getNumOperands() << "\n";

          for (unsigned int i = 0; i < SI->getNumOperands(); ++i) {
            if (ConstantInt *CI = dyn_cast<ConstantInt>(SI->getOperand(i))) {
              errs() << "Constant int dans un store: " << CI->getValue() <<"\n";
              Value *mbrExpr;
              errs() << "Valeur en int dans le Bin op: " << CI->getValue().getLimitedValue() << " Value de base: " << CI->getValue() << "\n";

              if ( i % 2 ) {
                ConstantInt *CI2 = dyn_cast<ConstantInt>(SI->getOperand(1));
                mbrExpr = AddStoreMBALinearOperation(*SI, MBAOperation(CI->getValue().getLimitedValue()), CI2, SI->getOperand(i)->getType());
              } else {
                ConstantInt *CI2 = dyn_cast<ConstantInt>(SI->getOperand(0));
                mbrExpr = AddStoreMBALinearOperation(*SI, MBAOperation(CI->getValue().getLimitedValue()), CI2, SI->getOperand(i)->getType());
              }
              errs() << "SetOp crash\n";
              SI->setOperand(i, mbrExpr);
            }
          }
        }
      }

      return true;
    }

  };
}

static void registerPass(const PassManagerBuilder &,legacy::PassManagerBase &PM) {
    PM.add(new OmniPass());
}

static RegisterStandardPasses
    RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                   registerPass);

char OmniPass::ID = 0;
static RegisterPass<OmniPass>

Y("Omni", "LLVM obfuscation for Omniscian");
