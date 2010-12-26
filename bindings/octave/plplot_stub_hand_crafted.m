# Hand-crafted file to drop plplot_octave name space.
# The first approximation to this file was obtained from the 
# the matwrap-generated plplot_stub.m as follows:
# egrep "(^function|^endfunction|^  plplot_octave| = plplot_octave)" ../temp_traditional_bindings_octave/plplot_stub.m >|bindings/octave/plplot_stub_hand_crafted.m
# Further editing done under emacs with a keyboard macro to
# replace numerical calls to plplot_octave with namespace ones.
# Returned functions not transformed yet.

1;

function pl_setcontlabelformat(lexp, sigdig)
  plplot_octave.pl_setcontlabelformat(lexp, sigdig);
endfunction
function pl_setcontlabelparam(offset, size, spacing, active)
  plplot_octave.pl_setcontlabelparam(offset, size, spacing, active);
endfunction
function pladv(page)
  plplot_octave.pladv(page);
endfunction
function plarc(x, y, a, b, angle1, angle2, fill)
  plplot_octave.plarc(x, y, a, b, angle1, angle2, fill);
endfunction
function plaxes(x0, y0, xopt, xtick, nxsub, yopt, ytick, nysub)
  plplot_octave.plaxes(x0, y0, xopt, xtick, nxsub, yopt, ytick, nysub);
endfunction
function plbin(x, y, center)
  plplot_octave.plbin(x, y, center);
endfunction
function plbop()
  plplot_octave.plbop();
endfunction
function plbox(xopt, xtick, nxsub, yopt, ytick, nysub)
  plplot_octave.plbox(xopt, xtick, nxsub, yopt, ytick, nysub);
endfunction
function plbox3(xopt, xlabel, xtick, nsubx, yopt, ylabel, ytick, nsuby, zopt, zlabel, ztick, nsubz)
  plplot_octave.plbox3(xopt, xlabel, xtick, nsubx, yopt, ylabel, ytick, nsuby, zopt, zlabel, ztick, nsubz);
endfunction
function [year, month, day, hour, min, sec] = plbtime(ctime)
  [year, month, day, hour, min, sec] = plplot_octave(9, ctime);
endfunction
function [wx, wy, window] = plcalc_world(rx, ry)
    [wx, wy, window] = plplot_octave(10, rx, ry);
endfunction
function plclear()
  plplot_octave.plclear();
endfunction
function plcol0(icol0)
  plplot_octave.plcol0(icol0);
endfunction
function plcol1(col1)
  plplot_octave.plcol1(col1);
endfunction
function plconfigtime(scale, offset1, offset2, ccontrol, ifbtime_offset, year, month, day, hour, min, sec)
  plplot_octave.plconfigtime(scale, offset1, offset2, ccontrol, ifbtime_offset, year, month, day, hour, min, sec);
endfunction
function plcpstrm(iplsr, flags)
  plplot_octave.plcpstrm(iplsr, flags);
endfunction
function ctime = plctime(year, month, day, hour, min, sec)
  ctime = plplot_octave(16, year, month, day, hour, min, sec);
endfunction
function plend()
plplot_octave.plgra();
endfunction
function plend1()
  plplot_octave.plend1();
endfunction
function plenv(xmin, xmax, ymin, ymax, just, axis)
  plplot_octave.plenv(xmin, xmax, ymin, ymax, just, axis);
endfunction
function plenv0(xmin, xmax, ymin, ymax, just, axis)
  plplot_octave.plenv0(xmin, xmax, ymin, ymax, just, axis);
endfunction
function pleop()
  plplot_octave.pleop();
endfunction
function plerrx(xmin, xmax, y)
  plplot_octave.plerrx(xmin, xmax, y);
endfunction
function plerry(x, ymin, ymax)
  plplot_octave.plerry(x, ymin, ymax);
endfunction
function plfamadv()
  plplot_octave.plfamadv();
endfunction
function plfill(x, y)
  plplot_octave.plfill(x, y);
endfunction
function plfill3(x, y, z)
  plplot_octave.plfill3(x, y, z);
endfunction
function plflush()
  plplot_octave.plflush();
endfunction
function plfont(ifont)
  plplot_octave.plfont(ifont);
endfunction
function plfontld(fnt)
  plplot_octave.plfontld(fnt);
endfunction
function [p_def, p_ht] = plgchr()
  [p_def, p_ht] = plplot_octave(30);
endfunction
function [r, g, b] = plgcol0(icol0)
  [r, g, b] = plplot_octave(31, icol0);
endfunction
function [r, g, b, a] = plgcol0a(icol0)
  [r, g, b, a] = plplot_octave(32, icol0);
endfunction
function [r, g, b] = plgcolbg()
  [r, g, b] = plplot_octave(33);
endfunction
function [r, g, b, a] = plgcolbga()
  [r, g, b, a] = plplot_octave(34);
endfunction
function compression = plgcompression()
  compression = plplot_octave(35);
endfunction
function p_dev = plgdev()
  p_dev = plplot_octave(36);
endfunction
function [p_mar, p_aspect, p_jx, p_jy] = plgdidev()
  [p_mar, p_aspect, p_jx, p_jy] = plplot_octave(37);
endfunction
function p_rot = plgdiori()
  p_rot = plplot_octave(38);
endfunction
function [p_xmin, p_ymin, p_xmax, p_ymax] = plgdiplt()
  [p_xmin, p_ymin, p_xmax, p_ymax] = plplot_octave(39);
endfunction
function [p_fam, p_num, p_bmax] = plgfam()
  [p_fam, p_num, p_bmax] = plplot_octave(40);
endfunction
function pfci = plgfci()
  pfci = plplot_octave(41);
endfunction
function fnam = plgfnam()
  fnam = plplot_octave(42);
endfunction
function [p_family, p_style, p_weight] = plgfont()
  [p_family, p_style, p_weight] = plplot_octave(43);
endfunction
function p_level = plglevel()
  p_level = plplot_octave(44);
endfunction
function [p_xp, p_yp, p_xleng, p_yleng, p_xoff, p_yoff] = plgpage()
  [p_xp, p_yp, p_xleng, p_yleng, p_xoff, p_yoff] = plplot_octave(45);
endfunction
function plgra()
plplot_octave.plgra();
endfunction
function plgradient(x, y, angle)
  plplot_octave.plgradient(x, y, angle);
endfunction
function [xmin, xmax, ymin, ymax] = plgspa()
  [xmin, xmax, ymin, ymax] = plplot_octave(48);
endfunction
function p_strm = plgstrm()
  p_strm = plplot_octave(49);
endfunction
function p_ver = plgver()
  p_ver = plplot_octave(50);
endfunction
function [p_xmin, p_xmax, p_ymin, p_ymax] = plgvpd()
  [p_xmin, p_xmax, p_ymin, p_ymax] = plplot_octave(51);
endfunction
function [p_xmin, p_xmax, p_ymin, p_ymax] = plgvpw()
  [p_xmin, p_xmax, p_ymin, p_ymax] = plplot_octave(52);
endfunction
function [p_digmax, p_digits] = plgxax()
  [p_digmax, p_digits] = plplot_octave(53);
endfunction
function [p_digmax, p_digits] = plgyax()
  [p_digmax, p_digits] = plplot_octave(54);
endfunction
function [p_digmax, p_digits] = plgzax()
  [p_digmax, p_digits] = plplot_octave(55);
endfunction
function plhist(data, datmin, datmax, nbin, oldwin)
  plplot_octave.plhist(data, datmin, datmax, nbin, oldwin);
endfunction
function plhls(h, l, s)
  plplot_octave.plhls(h, l, s);
endfunction
function [p_r, p_g, p_b] = plhlsrgb(h, l, s)
  [p_r, p_g, p_b] = plplot_octave(58, h, l, s);
endfunction
function plinit()
  plplot_octave.plinit();
endfunction
function pljoin(x1, y1, x2, y2)
  plplot_octave.pljoin(x1, y1, x2, y2);
endfunction
function pllab(xlabel, ylabel, tlabel)
  plplot_octave.pllab(xlabel, ylabel, tlabel);
endfunction
function pllightsource(x, y, z)
  plplot_octave.pllightsource(x, y, z);
endfunction
function plline(x, y)
  plplot_octave.plline(x, y);
endfunction
function plline3(x, y, z)
  plplot_octave.plline3(x, y, z);
endfunction
function pllsty(lin)
  plplot_octave.pllsty(lin);
endfunction
function p_strm = plmkstrm()
  p_strm = plplot_octave(66);
endfunction
function plmtex(side, disp, pos, just, text)
  plplot_octave. pend();
endfunction
function plmtex3(side, disp, pos, just, text)
  plplot_octave.plmtex3(side, disp, pos, just, text);
endfunction
function retval = c_plparseopts(p_argc, argv, mode)
  retval = plplot_octave(69, p_argc, argv, mode);
endfunction
function plpat(inc, del)
  plplot_octave.plpat(inc, del);
endfunction
function plpoin(x, y, code)
  plplot_octave.plpoin(x, y, code);
endfunction
function plpoin3(x, y, z, code)
  plplot_octave.plpoin3(x, y, z, code);
endfunction
function plprec(setp, prec)
  plplot_octave.plprec(setp, prec);
endfunction
function plpsty(patt)
  plplot_octave.plpsty(patt);
endfunction
function plptex(x, y, dx, dy, just, text)
  plplot_octave.plptex(x, y, dx, dy, just, text);
endfunction
function plptex3(wx, wy, wz, dx, dy, dz, sx, sy, sz, just, text)
  plplot_octave.plptex3(wx, wy, wz, dx, dy, dz, sx, sy, sz, just, text);
endfunction
function retval = plrandd()
  retval = plplot_octave(77);
endfunction
function plreplot()
  plplot_octave.plreplot();
endfunction
function plrgb(r, g, b)
  plplot_octave.plrgb(r, g, b);
endfunction
function plrgb1(r, g, b)
  plplot_octave.plrgb1(r, g, b);
endfunction
function [p_h, p_l, p_s] = plrgbhls(r, g, b)
  [p_h, p_l, p_s] = plplot_octave(81, r, g, b);
endfunction
function plschr(def, scale)
  plplot_octave.plschr(def, scale);
endfunction
function plscmap0(r, g, b)
  plplot_octave.plscmap0(r, g, b);
endfunction
function plscmap0a(r, g, b, a)
  plplot_octave.plscmap0a(r, g, b, a);
endfunction
function plscmap0n(ncol0)
  plplot_octave.plscmap0n(ncol0);
endfunction
function plscmap1(r, g, b)
  plplot_octave.plscmap1(r, g, b);
endfunction
function plscmap1a(r, g, b, a)
  plplot_octave.plscmap1a(r, g, b, a);
endfunction
function plscmap1l(itype, intensity, coord1, coord2, coord3, rev)
  plplot_octave.plscmap1l(itype, intensity, coord1, coord2, coord3, rev);
endfunction
function plscmap1la(itype, intensity, coord1, coord2, coord3, a, rev)
  plplot_octave.plscmap1la(itype, intensity, coord1, coord2, coord3, a, rev);
endfunction
function plscmap1n(ncol1)
  plplot_octave.plscmap1n(ncol1);
endfunction
function plscol0(icol0, r, g, b)
  plplot_octave.plscol0(icol0, r, g, b);
endfunction
function plscol0a(icol0, r, g, b, a)
  plplot_octave.plscol0a(icol0, r, g, b, a);
endfunction
function plscolbg(r, g, b)
  plplot_octave.plscolbg(r, g, b);
endfunction
function plscolbga(r, g, b, a)
  plplot_octave.plscolbga(r, g, b, a);
endfunction
function plscolor(color)
  plplot_octave.plscolor(color);
endfunction
function plscompression(compression)
  plplot_octave.plscompression(compression);
endfunction
function plsdev(devname)
  plplot_octave.plsdev(devname);
endfunction
function plsdidev(mar, aspect, jx, jy)
  plplot_octave.plsdidev(mar, aspect, jx, jy);
endfunction
function plsdimap(dimxmin, dimxmax, dimymin, dimymax, dimxpmm, dimypmm)
  plplot_octave.plsdimap(dimxmin, dimxmax, dimymin, dimymax, dimxpmm, dimypmm);
endfunction
function plsdiori(rot)
  plplot_octave.plsdiori(rot);
endfunction
function plsdiplt(xmin, ymin, xmax, ymax)
  plplot_octave.plsdiplt(xmin, ymin, xmax, ymax);
endfunction
function plsdiplz(xmin, ymin, xmax, ymax)
  plplot_octave.plsdiplz(xmin, ymin, xmax, ymax);
endfunction
function plseed(s)
  plplot_octave.plseed(s);
endfunction
function plsesc(esc)
  plplot_octave.plsesc(esc);
endfunction
function plsfam(fam, num, bmax)
  plplot_octave.plsfam(fam, num, bmax);
endfunction
function plsfci(fci)
  plplot_octave.plsfci(fci);
endfunction
function plsfnam(fnam)
  plplot_octave.plsfnam(fnam);
endfunction
function plsfont(family, style, weight)
  plplot_octave.plsfont(family, style, weight);
endfunction
function plsmaj(def, scale)
  plplot_octave.plsmaj(def, scale);
endfunction
function plsmin(def, scale)
  plplot_octave.plsmin(def, scale);
endfunction
function plsori(ori)
  plplot_octave.plsori(ori);
endfunction
function plspage(xp, yp, xleng, yleng, xoff, yoff)
  plplot_octave.plspage(xp, yp, xleng, yleng, xoff, yoff);
endfunction
function plspal0(filename)
  plplot_octave.plspal0(filename);
endfunction
function plspal1(filename, interpolate)
  plplot_octave.plspal1(filename, interpolate);
endfunction
function plspause(pause)
  plplot_octave.plspause(pause);
endfunction
function plsstrm(strm)
  plplot_octave.plsstrm(strm);
endfunction
function plssub(nx, ny)
  plplot_octave.plssub(nx, ny);
endfunction
function plssym(def, scale)
  plplot_octave.plssym(def, scale);
endfunction
function plstar(nx, ny)
  plplot_octave.plstar(nx, ny);
endfunction
function plstart(devname, nx, ny)
  plplot_octave.plstart(devname, nx, ny);
endfunction
function plstripa(id, pen, x, y)
  plplot_octave.plstripa(id, pen, x, y);
endfunction
function plstripd(id)
  plplot_octave.plstripd(id);
endfunction
function plstyl(mark, space)
  plplot_octave.plstyl(mark, space);
endfunction
function plsvect(arrowx, arrowy, fill)
  plplot_octave.plsvect(arrowx, arrowy, fill);
endfunction
function plsvpa(xmin, xmax, ymin, ymax)
  plplot_octave.plsvpa(xmin, xmax, ymin, ymax);
endfunction
function plsxax(digmax, digits)
  plplot_octave.plsxax(digmax, digits);
endfunction
function plsyax(digmax, digits)
  plplot_octave.plsyax(digmax, digits);
endfunction
function plsym(x, y, code)
  plplot_octave.plsym(x, y, code);
endfunction
function plszax(digmax, digits)
  plplot_octave.plszax(digmax, digits);
endfunction
function pltext()
  plplot_octave.pltext();
endfunction
function pltimefmt(fmt)
  plplot_octave.pltimefmt(fmt);
endfunction
function plvasp(aspect)
  plplot_octave.plvasp(aspect);
endfunction
function plvpas(xmin, xmax, ymin, ymax, aspect)
  plplot_octave.plvpas(xmin, xmax, ymin, ymax, aspect);
endfunction
function plvpor(xmin, xmax, ymin, ymax)
  plplot_octave.plvpor(xmin, xmax, ymin, ymax);
endfunction
function plvsta()
  plplot_octave.plvsta();
endfunction
function plw3d(basex, basey, height, xmin0, xmax0, ymin0, ymax0, zmin0, zmax0, alt, az)
  plplot_octave.plw3d(basex, basey, height, xmin0, xmax0, ymin0, ymax0, zmin0, zmax0, alt, az);
endfunction
function plwid(width)
  plplot_octave.plwid(width);
endfunction
function plwind(xmin, xmax, ymin, ymax)
  plplot_octave.plwind(xmin, xmax, ymin, ymax);
endfunction
function status = plxormod(mode)
  status = plplot_octave(139, mode);
endfunction
function [retval, state, keysym, button, string, pX, pY, dX, dY, wX, wY, subwin] = plGetCursor()
  [retval, state, keysym, button, string, pX, pY, dX, dY, wX, wY, subwin] = plplot_octave(140);
endfunction
function [retval, x, y] = plTranslateCursor(x_in, y_in)
  [retval, x, y] = plplot_octave(141, x_in, y_in);
endfunction
function plcol(icol0)
  plplot_octave.plcol(icol0);
endfunction
function plcont(f, kx, lx, ky, ly, clevel, tr)
  plplot_octave.plcont(f, kx, lx, ky, ly, clevel, tr);
endfunction
function plcont0(f, kx, lx, ky, ly, clevel)
  plplot_octave.plcont0(f, kx, lx, ky, ly, clevel);
endfunction
function plcont1(f, kx, lx, ky, ly, clevel, xg, yg)
  plplot_octave.plcont1(f, kx, lx, ky, ly, clevel, xg, yg);
endfunction
function plcont2(f, kx, lx, ky, ly, clevel, xg, yg)
  plplot_octave.plcont2(f, kx, lx, ky, ly, clevel, xg, yg);
endfunction
function plcont2p(f, kx, lx, ky, ly, clevel, xg, yg)
  plplot_octave.plcont2p(f, kx, lx, ky, ly, clevel, xg, yg);
endfunction
function zg = plgriddata(x, y, z, xg, yg, type, data)
  zg = plplot_octave(148, x, y, z, xg, yg, type, data);
endfunction
function pplimage(a, xmin, xmax, ymin, ymax, zmin, zmax, dxmin, dxmax, dymin, dymax)
  plplot_octave.pplimage(a, xmin, xmax, ymin, ymax, zmin, zmax, dxmin, dxmax, dymin, dymax);
endfunction
function plimagefr(a, xmin, xmax, ymin, ymax, zmin, zmax, valuemin, valuemax)
  plplot_octave.plimagefr(a, xmin, xmax, ymin, ymax, zmin, zmax, valuemin, valuemax);
endfunction
function plimagefr1(a, xmin, xmax, ymin, ymax, zmin, zmax, valuemin, valuemax, xg, yg)
  plplot_octave.plimagefr1(a, xmin, xmax, ymin, ymax, zmin, zmax, valuemin, valuemax, xg, yg);
endfunction
function plimagefr2(a, xmin, xmax, ymin, ymax, zmin, zmax, valuemin, valuemax, xg, yg)
  plplot_octave.plimagefr2(a, xmin, xmax, ymin, ymax, zmin, zmax, valuemin, valuemax, xg, yg);
endfunction
function plimagefrx(a, xmin, xmax, ymin, ymax, zmin, zmax, valuemin, valuemax, tr)
  plplot_octave.plimagefrx(a, xmin, xmax, ymin, ymax, zmin, zmax, valuemin, valuemax, tr);
endfunction
function plmesh(x, y, z, opt)
  plplot_octave.plmesh(x, y, z, opt);
endfunction
function plmeshc(x, y, z, opt, clevel)
  plplot_octave.plmeshc(x, y, z, opt, clevel);
endfunction
function plot3d(x, y, z, opt, side)
  plplot_octave.plot3d(x, y, z, opt, side);
endfunction
function plot3dc(x, y, z, opt, clevel)
  plplot_octave.plot3dc(x, y, z, opt, clevel);
endfunction
function plpoly3(x, y, z, draw, clockwise)
  plplot_octave.plpoly3(x, y, z, draw, clockwise);
endfunction
function plshade(a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, tr)
  plplot_octave.plshade(a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, tr);
endfunction
function plshade1(a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, xg, yg)
  plplot_octave.plshade1(a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, xg, yg);
endfunction
function plshade2(a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, xg, yg)
  plplot_octave.plshade2(a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, xg, yg);
endfunction
function plshades(a, left, right, bottom, top, clevel, fill_width, cont_color, cont_width, rectangular)
  plplot_octave.plshades(a, left, right, bottom, top, clevel, fill_width, cont_color, cont_width, rectangular);
endfunction
function plshades1(a, left, right, bottom, top, clevel, fill_width, cont_color, cont_width, rectangular, xg, yg)
  plplot_octave.plshades1(a, left, right, bottom, top, clevel, fill_width, cont_color, cont_width, rectangular, xg, yg);
endfunction
function plshades2(a, left, right, bottom, top, clevel, fill_width, cont_color, cont_width, rectangular, xg, yg)
  plplot_octave.plshades2(a, left, right, bottom, top, clevel, fill_width, cont_color, cont_width, rectangular, xg, yg);
endfunction
function plshadesx(a, left, right, bottom, top, clevel, fill_width, cont_color, cont_width, rectangular, tr)
  plplot_octave.plshadesx(a, left, right, bottom, top, clevel, fill_width, cont_color, cont_width, rectangular, tr);
endfunction
function id = plstripc(xspec, yspec, xmin, xmax, xjump, ymin, ymax, xlpos, ylpos, y_ascl, acc, colbox, collab, colline, styline, legline1, legline2, legline3, legline4, labx, laby, labtop)
  id = plplot_octave(166, xspec, yspec, xmin, xmax, xjump, ymin, ymax, xlpos, ylpos, y_ascl, acc, colbox, collab, colline, styline, legline1, legline2, legline3, legline4, labx, laby, labtop);
endfunction
function plsurf3d(x, y, z, opt, clevel)
  plplot_octave.plsurf3d(x, y, z, opt, clevel);
endfunction
function plvect(u, v, scale, tr)
  plplot_octave.plvect(u, v, scale, tr);
endfunction
function plvect1(u, v, scale, xg, yg)
  plplot_octave.plvect1(u, v, scale, xg, yg);
endfunction
function plvect2(u, v, scale, xg, yg)
  plplot_octave.plvect2(u, v, scale, xg, yg);
endfunction
function plClearOpts()
  plplot_octave.plClearOpts();
endfunction
function retval = plGetFlt(s)
  retval = plplot_octave(172, s);
endfunction
function retval = plGetInt(s)
  retval = plplot_octave(173, s);
endfunction
function plOptUsage()
  plplot_octave.plOptUsage();
endfunction
function plResetOpts()
  plplot_octave.plResetOpts();
endfunction
function retval = plSetOpt(opt, optarg)
  retval = plplot_octave(176, opt, optarg);
endfunction
function plSetUsage(program_string, usage_string)
  plplot_octave.plSetUsage(program_string, usage_string);
endfunction
function plarrows(u, v, x, y, scale, dx, dy)
  plplot_octave.plarrows(u, v, x, y, scale, dx, dy);
endfunction
function pldid2pc(xmin, ymin, xmax, ymax)
  plplot_octave.pldid2pc(xmin, ymin, xmax, ymax);
endfunction
function pldip2dc(xmin, ymin, xmax, ymax)
  plplot_octave.pldip2dc(xmin, ymin, xmax, ymax);
endfunction
function plsError(errcode, errmsg)
  plplot_octave.plsError(errcode, errmsg);
endfunction
function plsxwin(window_id)
  plplot_octave.plsxwin(window_id);
endfunction
