import { create } from "zustand";

export const useStore = create((set) => ({
  
  porcentajeDeCorte: 50,
  miliSegundosBomba: 5000,
  subePorentajeDeCorte: () =>
    set((state) => ({ porcentajeDeCorte: state.porcentajeDeCorte + 1 })),
  bajaPorentajeDeCorte: () =>
    set((state) => ({ porcentajeDeCorte: state.porcentajeDeCorte - 1 })),
  agregaSegundosBomba: (milis) =>
    set((state) => ({ miliSegundosBomba: milis })),
}));
