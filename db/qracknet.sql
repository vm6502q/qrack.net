--
-- PostgreSQL database dump
--

-- Dumped from database version 12.17 (Ubuntu 12.17-0ubuntu0.20.04.1)
-- Dumped by pg_dump version 12.17 (Ubuntu 12.17-0ubuntu0.20.04.1)

-- Started on 2024-01-18 18:38:13 EST

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 202 (class 1259 OID 18088)
-- Name: jobStatusTypes; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."jobStatusTypes" (
    id integer NOT NULL,
    name text NOT NULL
);


ALTER TABLE public."jobStatusTypes" OWNER TO postgres;

--
-- TOC entry 203 (class 1259 OID 18094)
-- Name: jobs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.jobs (
    id integer NOT NULL,
    "jobStatusTypeId" integer NOT NULL,
    "statusMessage" text,
    "userId" integer NOT NULL,
    "createdAt" date NOT NULL,
    "updatedAt" date
);


ALTER TABLE public.jobs OWNER TO postgres;

--
-- TOC entry 204 (class 1259 OID 18097)
-- Name: outputTypes; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."outputTypes" (
    id integer NOT NULL,
    name text NOT NULL,
    "createdAt" date NOT NULL,
    "updatedAt" date
);


ALTER TABLE public."outputTypes" OWNER TO postgres;

--
-- TOC entry 205 (class 1259 OID 18103)
-- Name: outputs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.outputs (
    id integer NOT NULL,
    "jobId" integer NOT NULL,
    "outputTypeId" integer NOT NULL,
    name text NOT NULL,
    value text NOT NULL,
    "createdAt" date NOT NULL,
    "updatedAt" date
);


ALTER TABLE public.outputs OWNER TO postgres;

--
-- TOC entry 206 (class 1259 OID 18109)
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    id integer NOT NULL,
    username text NOT NULL,
    "usernameNormal" text NOT NULL,
    "passwordHash" text NOT NULL,
    affiliation text NOT NULL,
    name text NOT NULL,
    email text NOT NULL,
    "recoveryToken" text NOT NULL,
    "recoveryTokenExpiration" date NOT NULL,
    "createdAt" date NOT NULL,
    "updatedAt" date
);


ALTER TABLE public.users OWNER TO postgres;

--
-- TOC entry 2989 (class 0 OID 18088)
-- Dependencies: 202
-- Data for Name: jobStatusTypes; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."jobStatusTypes" (id, name) FROM stdin;
1	SUCCESS
2	FAILED
3	RUNNING
4	IN QUEUE
\.


--
-- TOC entry 2990 (class 0 OID 18094)
-- Dependencies: 203
-- Data for Name: jobs; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.jobs (id, "jobStatusTypeId", "statusMessage", "userId", "createdAt", "updatedAt") FROM stdin;
\.


--
-- TOC entry 2991 (class 0 OID 18097)
-- Dependencies: 204
-- Data for Name: outputTypes; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."outputTypes" (id, name, "createdAt", "updatedAt") FROM stdin;
\.


--
-- TOC entry 2992 (class 0 OID 18103)
-- Dependencies: 205
-- Data for Name: outputs; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.outputs (id, "jobId", "outputTypeId", name, value, "createdAt", "updatedAt") FROM stdin;
\.


--
-- TOC entry 2993 (class 0 OID 18109)
-- Dependencies: 206
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.users (id, username, "usernameNormal", "passwordHash", affiliation, name, email, "recoveryToken", "recoveryTokenExpiration", "createdAt", "updatedAt") FROM stdin;
\.


--
-- TOC entry 2852 (class 2606 OID 18116)
-- Name: jobs jobs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_pkey PRIMARY KEY (id);


--
-- TOC entry 2854 (class 2606 OID 18118)
-- Name: outputTypes outputTypes_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."outputTypes"
    ADD CONSTRAINT "outputTypes_pkey" PRIMARY KEY (id);


--
-- TOC entry 2856 (class 2606 OID 18120)
-- Name: outputs outputs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_pkey PRIMARY KEY (id);


--
-- TOC entry 2850 (class 2606 OID 18122)
-- Name: jobStatusTypes statusTypes_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."jobStatusTypes"
    ADD CONSTRAINT "statusTypes_pkey" PRIMARY KEY (id);


--
-- TOC entry 2858 (class 2606 OID 18124)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- TOC entry 2859 (class 2606 OID 18125)
-- Name: jobs jobs_jobstatustypeid_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_jobstatustypeid_fk FOREIGN KEY ("jobStatusTypeId") REFERENCES public."jobStatusTypes"(id) NOT VALID;


--
-- TOC entry 2860 (class 2606 OID 18130)
-- Name: jobs jobs_userid_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_userid_fk FOREIGN KEY ("userId") REFERENCES public.users(id) ON DELETE CASCADE NOT VALID;


--
-- TOC entry 2861 (class 2606 OID 18135)
-- Name: outputs outputs_jobid_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_jobid_fk FOREIGN KEY ("jobId") REFERENCES public.jobs(id) NOT VALID;


--
-- TOC entry 2862 (class 2606 OID 18140)
-- Name: outputs outputs_typeid_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_typeid_fk FOREIGN KEY ("outputTypeId") REFERENCES public."outputTypes"(id) NOT VALID;


-- Completed on 2024-01-18 18:38:13 EST

--
-- PostgreSQL database dump complete
--

