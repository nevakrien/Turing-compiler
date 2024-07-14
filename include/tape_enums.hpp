#ifndef TAPE_ENUMS_HPP
#define TAPE_ENUMS_HPP

enum class TapeVal {
    Unchanged = 0,
    Allways1,
    Allways0,
    Flip,
    Unknown,
};

enum class RunTimeVal{
    Unknown=-1,
    Zero=0,
    One
};

static inline TapeVal combine_tapevals(TapeVal original,TapeVal op){
    
    //uname spacing tapeval
    constexpr TapeVal Unchanged = TapeVal::Unchanged;
    constexpr TapeVal Allways1=TapeVal::Allways1;
    constexpr TapeVal Allways0=TapeVal::Allways0;
    constexpr TapeVal Flip=TapeVal::Flip;
    constexpr TapeVal Unknown = TapeVal::Unknown;

    switch(op){
        case TapeVal::Unchanged:
            return original;

        case TapeVal::Allways1:
        case TapeVal::Allways0:
        case TapeVal::Unknown:
            return op;
        case Flip:
            break;
    }

    switch(original){
        case Allways1:
            return Allways0;
        case Allways0:
            return Allways1;
        case Flip:
            return Unchanged;
        case Unchanged:
            return Flip;
        case Unknown:
            return Unknown;
        default:
            UNREACHABLE();
    }
}



static inline TapeVal run_to_tape(RunTimeVal val){
    switch(val){
        case RunTimeVal::Unknown:
            return TapeVal::Unknown;
        case RunTimeVal::Zero:
            return TapeVal::Allways0;
        case RunTimeVal::One:
            return TapeVal::Allways1;
        default: 
            UNREACHABLE();
    }
    return TapeVal::Unknown;
}

static inline RunTimeVal run_tapeval(RunTimeVal val, TapeVal tape){
    switch(tape){
        case TapeVal::Unchanged:
            return val;
        case TapeVal::Allways1:
            return RunTimeVal::One;
        case TapeVal::Allways0:
            return RunTimeVal::Zero;
        case TapeVal::Flip:
            if(val==RunTimeVal::Unknown){
                return val;
            }
            return (RunTimeVal)(1-(int)val);
            
        case TapeVal::Unknown:
            return RunTimeVal::Unknown;
        default: 
            UNREACHABLE();
    }
    return RunTimeVal::Unknown;
}



#endif // TAPE_ENUMS_HPP
